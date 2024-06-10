struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};



cbuffer WorldBuffer : register(b0)
{
	float4x4 _world;
};

cbuffer PSBuffer : register(b2)
{
	float totalTime;
    float enabled;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 _view;
	float4x4 _projection;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;
	return output;
}

Texture2D _sourceTex : register(t0);
Texture2D _sourceTex2 : register(t1);
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
	float4 color = _sourceTex.Sample(_samp, input.uv);
	if (enabled)
		color.a = 0.2f + abs(sin(2.7f * totalTime))* color.a;
	else
        color.a = 0.1f;
	return color;

}