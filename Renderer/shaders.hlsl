cbuffer SceneData : register(b0)
{
    float4x4 WVP;
}

// PSInput - Input to the pixel shader
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

// Texture and sampler
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// VSMain - Input to the vertex shader from the vertex buffer
PSInput VSMain(VSInput _input)
{
    PSInput result;

    result.position = mul(float4(_input.position, 1.0f), WVP);
    result.color = _input.color;
    result.uv = _input.uv;

    return result;
}

// PSMain - Pixel shader
float4 PSMain(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.uv);
}