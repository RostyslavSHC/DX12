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
Texture2D g_texture1 : register(t1);
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
    float4 brickColor = g_diffuse.Sample(g_sampler, input.uv);
    float4 flowerColor = g_texture1.Sample(g_sampler, input.uv);

    // Mix brick with flowers
    float4 baseColor = lerp(brickColor, flowerColor, 0.25f);

    // Get the cube's center position in world space
    float3 cubePosition = float3(World[3][0], World[3][1], World[3][2]);

    // Calculate the distance from the starting position on each axis
    float3 distance = abs(cubePosition);

    // Calculate the position-based color
    float3 positionColor = saturate(distance / 2.0f);

    // Gradually increase the color effect as the cube moves
    float tintAmount = saturate(max(max(distance.x, distance.y), distance.z) / 2.0f);

    // Create a brighter position-based color while preserving texture details
    float3 tintedColor = baseColor.rgb * 0.45f + positionColor * 0.50f;

    // Blend original texture with the position-based color
    float3 finalColor = lerp(baseColor.rgb, tintedColor, tintAmount * 0.85f);

    return float4(saturate(finalColor), baseColor.a);
}