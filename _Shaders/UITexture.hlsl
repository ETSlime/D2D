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
	matrix _world;
};

cbuffer VPBuffer : register(b1)
{
	matrix _view;
	matrix _projection;
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

    if (color.a < 0.1)
        discard;

	return color;

}





//struct VertexInput
//{
//	float4 position : POSITION0;
//    float2 uv : TEXCOORD0;
//};
//
//struct PixelInput
//{
//	float4 position : SV_POSITION0;
//    float2 uv : TEXCOORD0;
//};
//
//cbuffer WorldBuffer : register(b0)// 0 ~ 127
//{
//    matrix _world;
//};
//
//cbuffer VPBuffer : register(b1)
//{
//    matrix _view;
//    matrix _projection;
//};
//
//cbuffer ShadedBuffer : register(b0)
//{
//	int Selection;
//}
//
//PixelInput VS(VertexInput input)
//{
//	PixelInput output;
//    
//    output.position = mul(input.position, _world);
//    output.position = mul(output.position, _view);
//    output.position = mul(output.position, _projection);
//    
//	output.uv = input.uv;
//	return output;
//}
//
//
//
//
//Texture2D _sourceTex : register(t0);
//
//SamplerState _samp : register(s0);
//
//float4 PS(PixelInput input) : SV_Target
//{
//	float4 color = _sourceTex.Sample(_samp, (float2)input.uv);
//		
//	if (Selection == 0)
//	{
//		if (input.uv.x < 0.05f || input.uv.x > 0.95f || input.uv.y < 0.05f || input.uv.y > 0.95f)
//			color = float4(1, 1, 1, 1);
//		else
//			float4 color = _sourceTex.Sample(_samp, (float2)input.uv);
//		return color;
//	}
//	else if (Selection == 1)
//	{
//		if (input.uv.x < 0.05f || input.uv.x > 0.95f || input.uv.y < 0.05f || input.uv.y > 0.95f)
//			color = float4(1, 1, 1, 1);
//		else
//			float4 color = _sourceTex.Sample(_samp, (float2)input.uv);
//		return color;
//	}
//    
//}