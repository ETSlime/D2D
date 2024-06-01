struct VertexInput
{
    float4 position : POSITION0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
};

cbuffer WorldBuffer : register(b0) // 0 ~ 127
{
    matrix _world;
};

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

cbuffer TimeBuffer : register(b0)
{
    float t;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _view);
    output.position = mul(output.position, _projection);

    return output;
}


float4 PS(PixelInput input) : SV_Target
{

    float4 color = float4(0.0f, 0.0f, 0.0f, 0.5f);
    color.a = t;
    return color;
}