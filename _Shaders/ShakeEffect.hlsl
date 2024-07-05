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


cbuffer shakeBuffer : register(b2)
{
    matrix shakeMatrix;
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
    output.position = mul(output.position, shakeMatrix); // Apply shake matrix
    output.texcoord = input.texcoord;
    
    return output;
}

// Pixel shader
Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(PS_INPUT input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.texcoord);
}
