cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    matrix View;
    matrix Projection;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

struct VSInput
{
    float3 pos : POSITION;
};

TextureCube g_skybox : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput output;

    matrix viewNoTranslation = View;
    viewNoTranslation[3] = float4(0, 0, 0, 1);
    float4 pos = mul(float4(input.pos, 1.0), viewNoTranslation);
    output.pos = mul(pos, Projection);
    output.dir = input.pos;

    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return g_skybox.Sample(g_sampler, input.dir);
}