cbuffer SceneData : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float4x4 InvWorld;

    float4 PlayerPosition;
    float Tagged;
    float RenderSpotlights;
    float SpotlightCount;
    float Padding;

    float4 SpotlightPositions[9];
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




float CalculateSpotlights(float3 _pixelWorld)
{
    const float radius = 4.0f;
    float intensity = 0.0f;

    for (int i = 0; i < (int) SpotlightCount; i++)
    {
        // Calculate the distance from the spotlight
        float dist = distance(_pixelWorld.xy, SpotlightPositions[i].xy);

        // Calculate the spotlight intensity
        if (dist < radius)
        {
            // intensity += 0.10f;
            // intensity += 0.05f;
            intensity += 0.075f;
        }
    }

    return saturate(intensity);
}













float4 PSMain(PSInput input) : SV_TARGET
{
    // Render tagged NPCs in solid yellow
    if (Tagged > 0.5f)
    {
        return float4(1.0f, 1.0f, 0.0f, 1.0f);
    }

    float4 color = g_texture.Sample(g_sampler, input.texcoords);

    // Calculate the distance from the pixel to the player
    float dist = distance(input.pixelWorld, PlayerPosition.xyz);

    // Calculate the amount of red and green tint
    float redTint = saturate((10.0f - dist) / 10.0f);
    float greenTint = saturate((dist - 10.0f) / 20.0f);

    // Reduce the ambient brightness
    if (RenderSpotlights > 0.5f)
    {
        color.rgb *= 0.5f;
    }

    // Apply the distance-based tint
    // color.r += redTint;
    // color.g += greenTint * 0.6f;
    // color.r += redTint * 1.15f;
    // color.r += redTint * 1.25f;
    color.r += redTint * 1.10f;

    // Reduce the green and blue components in the red area
    // color.g *= (1.0f - redTint * 0.20f);
    // color.b *= (1.0f - redTint * 0.20f);
    
    // Preserve bright areas of the original texture
    color.g *= (1.0f - redTint * 0.10f * (1.0f - color.g));
    color.b *= (1.0f - redTint * 0.10f * (1.0f - color.b));

    // color.g += greenTint * 0.75f;
    color.g += greenTint * 0.65f;

    // Apply white spotlights
    if (RenderSpotlights > 0.5f)
    {
        float intensity = CalculateSpotlights(input.pixelWorld);

        // color.rgb += intensity;
        color.rgb += g_texture.Sample(g_sampler, input.texcoords).rgb * intensity; // Apply white light based on the original texture color
    }

    return saturate(color);
}