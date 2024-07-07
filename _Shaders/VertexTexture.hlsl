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

cbuffer PSBuffer : register(b2)
{
    float enabled;
    float fadeIn;
    float fadeOut;
    float totalTime;
    float startTime;
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
	return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = _sourceTex.Sample(_samp, (float2)input.uv);
    if (!enabled)
        color = color * 0.5;
    
    // fade
    float fadeDuration = 1.0;
    
    // Calculate the relative time since the fade started
    float relativeTime = totalTime - startTime;
    
    // Only change the alpha value if the pixel is originally not transparent
    if (color.a > 0.5)
    {
        if (fadeIn)
        {
        // Fade in from 0 to original alpha
            color.a = saturate(relativeTime / fadeDuration);
        }
        else if (fadeOut)
        {
        // Fade out but ensure alpha does not go below a small threshold to distinguish from fully transparent pixels
            color.a = saturate(1.0 - (relativeTime / fadeDuration) * 0.99);
        }
    }

    
    return color;
}