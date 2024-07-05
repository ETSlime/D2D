// Vertex shader input and output structures
struct VS_INPUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

// Vertex shader constant buffer
cbuffer WorldBuffer : register(b0) // 0 ~ 127
{
    matrix _world;
};

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

// Vertex shader
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.texcoord = input.texcoord;
    
    return output;
}

// Pixel shader
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer BlurBuffer
{
    float2 texelSize;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    float2 offsets[5] =
    {
        float2(-2, 0), float2(-1, 0), float2(0, 0), float2(1, 0), float2(2, 0)
    };

    float weights[5] =
    {
        0.1, 0.2, 0.4, 0.2, 0.1
    };

    for (int i = 0; i < 5; ++i)
    {
        color += shaderTexture.Sample(SampleType, input.texcoord + offsets[i] * texelSize) * weights[i];
    }

    return color;
}
