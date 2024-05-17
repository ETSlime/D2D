#pragma once

#include "SelectorTextureRect.h"

SelectorTextureRect::SelectorTextureRect(DirectX::XMFLOAT3 position, 
	DirectX::XMFLOAT3 size, float rotation,std::wstring texturePath) 
	:RectBase<VertexTexture>(position, size, rotation)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"SelectorTexture.hlsl");
	shaderBuffer = new UploadBuffer<float>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);

	// SRV
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		mDevice,
		texturePath.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	);
	Assert(SUCCEEDED(hr));
}

SelectorTextureRect::~SelectorTextureRect()
{
	SafeRelease(&srv);
}

void SelectorTextureRect::SetVertices()
{
	vertices.assign(4, VertexTexture());

	vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0.667, 0.75);


	vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(0.834, 0.5);

	vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(0.834, 0.75);

	vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[3].uv = DirectX::XMFLOAT2(0.667, 0.5);
}

void SelectorTextureRect::SetIndices()
{
	indices = { 0, 1, 2, 0, 3, 1 };
}

void SelectorTextureRect::Update()
{
	totalTime = Timer::TotalTime();
	shaderBuffer->MapData(mDeviceContext, totalTime);
	UpdateWorld();
}

void SelectorTextureRect::Render()
{
	mDeviceContext->PSSetConstantBuffers(2, 1, shaderBuffer->Resource());
	__super::Render();
}