
cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    float4 DiffuseColor;
    float4 AmbientLight;
    float3 LightPosition;
    float pad0; // Padding to align to 16 bytes
    float4 LightColor;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
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
    result.normal = mul(float4(_input.normal, 1.0f), InvWorld);
    result.uv = _input.uv;
    result.pixelWorld = mul(float4(_input.position, 1.0f), World).xyz;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 lightDir = normalize(LightPosition - input.pixelWorld);
    float4 lambertian = clamp(dot(lightDir, input.normal), 0.0f, 1.0f);
    float4 color = g_diffuse.Sample(g_sampler, input.uv) * DiffuseColor * LightColor;
    float4 intensity = AmbientLight + lambertian;
    return color * intensity;
}
