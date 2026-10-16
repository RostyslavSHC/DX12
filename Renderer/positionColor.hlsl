cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 pixelWorld : POSITION0;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

Texture2D g_diffuse : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain(VSInput _input)
{
    PSInput result;

    result.position = mul(float4(_input.position, 1.0f), WVP);
    result.uv = _input.uv;
    result.pixelWorld = mul(float4(_input.position, 1.0f), World).xyz;

    return result;
}

/*
float4 PSMain(PSInput input) : SV_TARGET
{
    float4 textureColor = g_diffuse.Sample(g_sampler, input.uv);

    // Change the cube's tint based on its world position
    float3 tint = saturate((input.pixelWorld + 1.0f) / 2.0f);
    //float3 tint = saturate(0.2f + abs(input.pixelWorld) * 0.5f);

    return float4(textureColor.rgb * tint, textureColor.a);
}
*/




float4 PSMain(PSInput input) : SV_TARGET
{
    float4 textureColor = g_diffuse.Sample(g_sampler, input.uv);

    // Get the cube's center position in world space
    float3 cubePosition = float3(World[3][0], World[3][1], World[3][2]);

    // Calculate the distance from the starting position on each axis
    float3 distance = abs(cubePosition);

    // Start with the original texture color
    float3 tint = float3(1.0f, 1.0f, 1.0f);

    // Apply a position-based tint when the cube moves
    if (distance.x > 0.01f || distance.y > 0.01f || distance.z > 0.01f)
    {
        float3 positionColor = distance / max(max(distance.x, distance.y), distance.z);
        float tintAmount = saturate(max(max(distance.x, distance.y), distance.z));

        tint = lerp(float3(1.0f, 1.0f, 1.0f), positionColor, tintAmount);
    }

    return float4(textureColor.rgb * tint, textureColor.a);
}


