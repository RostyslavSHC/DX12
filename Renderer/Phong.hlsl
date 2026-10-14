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
    float SpecularStrength;
    float3 pad1;
    float4 SpecularColor;
    float4 CameraPosition;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 pixelWorld : POSITION0;
    float3 viewDirection : POSITION1;
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
    result.viewDirection = normalize(CameraPosition.xyz - result.pixelWorld);

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 lightDir = normalize(LightPosition - input.pixelWorld);
    float4 lambertian = clamp(dot(lightDir, input.normal), 0.0f, 1.0f);
    float3 refl = reflect(-lightDir, input.normal);
    float4 specColor = pow(max(dot(refl, input.viewDirection), 0.0f), SpecularStrength) * SpecularColor;
    float4 diffuseColor = g_diffuse.Sample(g_sampler, input.uv) * DiffuseColor * LightColor;
    float4 diffuseIntensity = AmbientLight + lambertian;
    return diffuseColor * diffuseIntensity + specColor;
}
