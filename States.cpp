#include "States.h"

D3D11_RASTERIZER_DESC		States::rsDesc;
D3D11_DEPTH_STENCIL_DESC	States::dsDesc;
D3D11_SAMPLER_DESC			States::spDesc;
D3D11_BLEND_DESC			States::blDesc;

void States::Create()
{
	CreateRasterizerDesc();
	CreateDepthStencilDesc();
	CreateSamplerDesc();
	CreateBlendDesc();
}

void States::GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc)
{
	memcpy(desc, &rsDesc, sizeof(D3D11_RASTERIZER_DESC));
}

void States::GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc)
{
	memcpy(desc, &dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
}

void States::GetSamplerDesc(D3D11_SAMPLER_DESC* desc)
{
	memcpy(desc, &spDesc, sizeof(D3D11_SAMPLER_DESC));
}

void States::GetBlendDesc(D3D11_BLEND_DESC* desc)
{
	memcpy(desc, &blDesc, sizeof(D3D11_BLEND_DESC));
}

void States::CreateRasterizer(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state)
{
	HRESULT hr = device->CreateRasterizerState(desc, state);
	Assert(SUCCEEDED(hr));
}

void States::CreateDepthStencil(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state)
{
	HRESULT hr = device->CreateDepthStencilState(desc, state);
	Assert(SUCCEEDED(hr));
}

void States::CreateSampler(ID3D11Device* device, D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state)
{
	HRESULT hr = device->CreateSamplerState(desc, state);
	Assert(SUCCEEDED(hr));
}

void States::CreateBlend(ID3D11Device* device, D3D11_BLEND_DESC* desc, ID3D11BlendState** state)
{
	HRESULT hr = device->CreateBlendState(desc, state);
	Assert(SUCCEEDED(hr));
}

void States::CreateRasterizerDesc()
{
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.AntialiasedLineEnable = false;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.MultisampleEnable = false;
	rsDesc.ScissorEnable = false;
	rsDesc.SlopeScaledDepthBias = 0.0f;
}

void States::CreateDepthStencilDesc()
{
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0XFF;
	dsDesc.StencilWriteMask = 0XFF;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

void States::CreateSamplerDesc()
{
	ZeroMemory(&spDesc, sizeof(D3D11_SAMPLER_DESC));
	spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.MaxAnisotropy = 1;
	spDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	spDesc.MinLOD = FLT_MIN;
	spDesc.MaxLOD = FLT_MAX;
}

void States::CreateBlendDesc()
{
	ZeroMemory(&blDesc, sizeof(D3D11_BLEND_DESC));
	blDesc.AlphaToCoverageEnable = false;
	blDesc.IndependentBlendEnable = false;

	blDesc.RenderTarget[0].BlendEnable = false;
	blDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}
