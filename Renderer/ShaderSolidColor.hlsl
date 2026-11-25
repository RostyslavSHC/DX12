cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    float4 color;
};

struct PSInput
{
    float4 position : SV_POSITION;
};

struct VSInput
{
    float3 position : POSITION;
};

PSInput VSMain(VSInput _input)
{
    PSInput result;
    result.position = mul(float4(_input.position, 1.0f), WVP);
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return color;
}