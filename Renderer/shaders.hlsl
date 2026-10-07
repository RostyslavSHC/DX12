// Data sent from C++ to shader. Now I get the matrix and YUV settings from C++
cbuffer SceneData : register(b0)
{
    float4x4 WVP;

    float yValue;
    float uValue;
    float vValue;
    float invertColors;
};

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
    // Get the RGB color
    float3 rgb = input.color.rgb;

    // Convert RGB to YUV
    float y = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
    float u = -0.14713 * rgb.r - 0.28886 * rgb.g + 0.436 * rgb.b;
    float v = 0.615 * rgb.r - 0.51499 * rgb.g - 0.10001 * rgb.b;

    // Apply the YUV values from the ToolsForm
    y *= yValue;
    u *= uValue;
    v *= vValue;

    // Convert YUV back to RGB
    rgb.r = y + 1.13983 * v;
    rgb.g = y - 0.39465 * u - 0.58060 * v;
    rgb.b = y + 2.03211 * u;

    // Invert the colors
    if (invertColors == 1.0f)
    {
        rgb = 1.0f - rgb;
    }

    return float4(rgb, input.color.a);
}
