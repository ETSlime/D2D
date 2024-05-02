#pragma once
#include "Util.h"

class States
{
public:
	static void Create();

	static void GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc);
	static void GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc);
	static void GetSamplerDesc(D3D11_SAMPLER_DESC* desc);
	static void GetBlendDesc(D3D11_BLEND_DESC* desc);

	static void CreateRasterizer(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state);
	static void CreateDepthStencil(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state);
	static void CreateSampler(ID3D11Device* device, D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state);
	static void CreateBlend(ID3D11Device* device, D3D11_BLEND_DESC* desc, ID3D11BlendState** state);

private:
	static void CreateRasterizerDesc();
	static void CreateDepthStencilDesc();
	static void CreateSamplerDesc();
	static void CreateBlendDesc();

	static D3D11_RASTERIZER_DESC rsDesc;
	static D3D11_DEPTH_STENCIL_DESC dsDesc;
	static D3D11_SAMPLER_DESC spDesc;
	static D3D11_BLEND_DESC blDesc;
};