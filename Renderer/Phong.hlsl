struct Material
{
    float4 DiffuseColor;
    float SpecularStrength;
    float3 pad1;
    float4 SpecularColor;
};

struct Light
{
    float4 AmbientColor;
    float3 Position;
    float pad0;
    float4 Color;
};

cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    Material material;
    Light light;
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
Texture2D g_specular : register(t1);
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
    input.normal = normalize(input.normal);
    float3 lightDir = normalize(light.Position - input.pixelWorld);
    float4 lambertianStrength = clamp(dot(lightDir, input.normal), 0.0f, 1.0f);
    float3 refl = reflect(-lightDir, input.normal);
    float4 specularStrength = pow(max(dot(refl, input.viewDirection), 0.0f), material.SpecularStrength);
    float4 ambientColor = g_diffuse.Sample(g_sampler, input.uv) * light.AmbientColor;
    float4 diffuseColor = lambertianStrength * g_diffuse.Sample(g_sampler, input.uv) * material.DiffuseColor;
    float4 specularColor = specularStrength * g_specular.Sample(g_sampler, input.uv) * material.SpecularColor;
    return ((ambientColor + diffuseColor + specularColor) * light.Color);
}
