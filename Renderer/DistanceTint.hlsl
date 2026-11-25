cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    float4 PlayerPosition;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORD0;
    float3 pixelWorld : POSITION0;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoords : TEXCOORD0;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain(VSInput _input)
{
    PSInput result;

    result.position = mul(float4(_input.position, 1.0f), WVP);
    result.texcoords = _input.texcoords;

    // Calculate the world position of the vertex
    result.pixelWorld = mul(float4(_input.position, 1.0f), World).xyz;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = g_texture.Sample(g_sampler, input.texcoords);

    // Calculate the distance from the pixel to the player
    float dist = distance(input.pixelWorld, PlayerPosition.xyz);

    // Calculate the amount of red and green tint
    float redTint = saturate((10.0f - dist) / 10.0f);
    float greenTint = saturate((dist - 10.0f) / 20.0f);

    // Apply the distance-based tint
    // color.r += redTint;
    // color.g += greenTint;
    
    color.r += redTint;
    color.g += greenTint * 0.6f;

    return saturate(color);
}