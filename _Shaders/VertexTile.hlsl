struct VertexInput
{
	float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float index : INDEX0;
    float bWalkable : WALKABLE0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float index : INDEX0;
    float bWalkable : WALKABLE0;
};

cbuffer WorldBuffer : register(b0)// 0 ~ 127
{
    matrix _world;
};

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
    
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    
	output.uv = input.uv;
    output.index = input.index;
    output.bWalkable = input.bWalkable;
	return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

cbuffer IndexNumBuffer : register(b0)
{
    uint _index;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    
    if (input.uv.x > 0.0f|| input.uv.y >0.0f)
        color = _sourceTex.Sample(_samp, input.uv);

    return color;
}