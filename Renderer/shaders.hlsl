cbuffer SceneData : register(b0)
{
    float4x4 WVP;
}

// PSInput - Input to the pixel shader
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// VSInput - Input to the vertex shader from the vertex buffer (input to the vertex shader)
PSInput VSMain(float3 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = mul(float4(position, 1.0f), WVP);
    result.color = color;

    return result;
}

// PSMain - Pixel shader (output to screen)
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}