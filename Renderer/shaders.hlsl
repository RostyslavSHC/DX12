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

// Textures and sampler
Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
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
    float4 col0 = g_texture0.Sample(g_sampler, input.uv);
    float4 col1 = g_texture1.Sample(g_sampler, input.uv);
    return lerp(col0, col1, 0.3);
}