#pragma once

#include "CursorTextureRect.h"

CursorTextureRect::CursorTextureRect(DirectX::XMFLOAT3 position, 
	DirectX::XMFLOAT3 size, float rotation,std::wstring texturePath) 
	:RectBase<VertexTexture>(position, size, rotation)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"CursorTexture.hlsl");
	shaderBuffer = new UploadBuffer<ShaderData>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
	shaderData.enabled = true;

	// SRV
	// Load texture from file using DirectXTex
	DirectX::ScratchImage image;
	HRESULT hr = LoadFromWICFile(texturePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	// Create texture resource
	ID3D11Texture2D* texture = nullptr;
	hr = CreateTexture(mDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), (ID3D11Resource**)&texture);
	// Create shader resource view
	hr = mDevice->CreateShaderResourceView(texture, nullptr, &srv);

	Assert(SUCCEEDED(hr));
}

CursorTextureRect::~CursorTextureRect()
{
	SafeRelease(&srv);
}

void CursorTextureRect::SetVertices()
{
	vertices.assign(4, VertexTexture());

	vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0.667f, 0.75f);


	vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(0.834f, 0.5f);

	vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(0.834f, 0.75f);

	vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[3].uv = DirectX::XMFLOAT2(0.667f, 0.5f);
}

void CursorTextureRect::SetIndices()
{
	indices = { 0, 1, 2, 0, 3, 1 };
}

void CursorTextureRect::Update()
{
	shaderData.totalTime = Timer::TotalTime();
	shaderBuffer->MapData(mDeviceContext, shaderData);
	UpdateWorld();
}

void CursorTextureRect::Render()
{
	mDeviceContext->PSSetConstantBuffers(2, 1, shaderBuffer->Resource());
	__super::Render();
}