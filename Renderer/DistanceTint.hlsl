cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORD0;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoords : TEXCOORD0;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput output;

    output.position = mul(float4(input.position, 1.0f), WVP);
    output.texcoords = input.texcoords;

    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.texcoords);
}