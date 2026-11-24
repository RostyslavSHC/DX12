struct Material
{
    float4 DiffuseColor;
    float SpecularStrength;
    int texIndDiffuse;
    int texIndSpecular;
    int texIndNormal;
    float4 SpecularColor;
};



#define MAX_LIGHTS 8
struct Light
{
    float4 AmbientColor;
    float3 Position;
    // float3 Direction;
    float pad0;
    float4 Color;
    float3 ConstLinQuad; // Pack 3 values: constant, linear, quadratic
    float pad1;
    float3 SpotDirection;
    float pad2;
    float2 SpotConeFalloff; // Pack 2 values: inner cone, outer cone
    float2 pad3;
};

cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;
    Material material;
    Light lights[MAX_LIGHTS];
    uint lightCount;
    float3 pad0;
    // Light light;
    float4 CameraPosition;
};


struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 pixelWorld : POSITION0;
    float3 viewDirection : POSITION1;
    float3x3 TBN : TANGENTSPACE;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT0;
    float3 bitangent : BITANGENT0;
};

#define MAX_TEXTURES 8
Texture2D g_Textures[MAX_TEXTURES] : register(t0);
SamplerState g_sampler : register(s0);

Texture2D g_diffuse : register(t0);
Texture2D g_specular : register(t1);

PSInput VSMain(VSInput _input)
{
    PSInput result;

    result.position = mul(float4(_input.position, 1.0f), WVP);
    result.normal = mul(float4(_input.normal, 1.0f), InvWorld);
    result.uv = _input.uv;
    result.pixelWorld = mul(float4(_input.position, 1.0f), World).xyz;
    result.viewDirection = normalize(CameraPosition.xyz - result.pixelWorld);
    
    float3 T = normalize(mul(World, float4(_input.tangent, 0.0))).xyz;
    float3 B = normalize(mul(World, float4(_input.bitangent, 0.0))).xyz;
    float3 N = normalize(mul(World, float4(_input.normal, 0.0))).xyz;
    result.TBN = float3x3(T, B, N);

    return result;
}



float4 SampleTexture(int index, float2 uv)
{
    if (index == -1) return float4(0, 0, 0, 1);
    if (index == 0) return g_Textures[0].Sample(g_sampler, uv);
    if (index == 1) return g_Textures[1].Sample(g_sampler, uv);
    if (index == 2) return g_Textures[2].Sample(g_sampler, uv);
    if (index == 3) return g_Textures[3].Sample(g_sampler, uv);
    if (index == 4) return g_Textures[4].Sample(g_sampler, uv);
    if (index == 5) return g_Textures[5].Sample(g_sampler, uv);
    if (index == 6) return g_Textures[6].Sample(g_sampler, uv);
    if (index == 7) return g_Textures[7].Sample(g_sampler, uv);
    return float4(1, 0, 1, 1); // debug pink
}


float4 PSMain(PSInput input) : SV_TARGET
{
    float4 diffColor = SampleTexture(material.texIndDiffuse, input.uv);
    if (diffColor.a == 0.0f)
    {
        discard;
    }

    input.normal = normalize(input.normal);
    float3 finalColor = float3(0.0f, 0.0f, 0.0f);
    for (uint count = 0; count < lightCount; count++)
    {
        
        if (material.texIndNormal != -1)
        {
            input.normal = SampleTexture(material.texIndNormal, input.uv).xyz;
            input.normal = input.normal * 2.0f - 1.0f;
            // float3x3 normalMatrix = (float3x3) World;
            input.normal = normalize(mul(input.normal, input.TBN));
        }
        
        float3 lightDir = normalize(lights[count].Position - input.pixelWorld);
        float4 LambertianStrength = clamp(dot(lightDir, input.normal), 0.0f, 1.0f);
        float3 refl = reflect(-lightDir, input.normal);
        float4 specularStrength = pow(max(dot(refl, input.viewDirection), 0.0f), material.SpecularStrength);
        float distance = length(lights[count].Position - input.pixelWorld);
        float att = 1.0f / (lights[count].ConstLinQuad[0] + lights[count].ConstLinQuad[1] * distance +
            lights[count].ConstLinQuad[2] * (distance * distance));
        float4 ambientColor = diffColor * lights[count].AmbientColor;
        float4 diffuseColor = LambertianStrength * diffColor * material.DiffuseColor * att;
        float4 specularColor = specularStrength * SampleTexture(material.texIndSpecular, input.uv) * material.SpecularColor * att;
        finalColor += ((ambientColor + diffuseColor + specularColor) * lights[count].Color).rgb;
    }
    return float4(finalColor, diffColor.a);
}