#include "UITextureRect.h"

UITextureRect::UITextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring texturePath)
	:RectBase(position, size, rotation)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"UITexture.hlsl");

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

void UITextureRect::SetVertices()
{
	float edgeSizeX = 18;
	float edgeSizeY = 8;
	float edgeScaleX = edgeSizeX / size.x;
	float edgeScaleY = edgeSizeY / size.y;

	vertices.assign(20, VertexTexture());

	vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 1.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0.667f, 0.5f);

	vertices[1].position = DirectX::XMFLOAT3(-0.5f + edgeScaleX, -0.5f + edgeScaleY, 0.0f);//DirectX::XMFLOAT3(-0.4f, -0.4f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(0.733f, 0.45f);

	vertices[2].position = DirectX::XMFLOAT3(-0.5f + edgeScaleX, -0.5f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(0.733f, 0.5f);

	vertices[3].position = DirectX::XMFLOAT3(-0.5f, -0.5f + edgeScaleY, 0.0f);
	vertices[3].uv = DirectX::XMFLOAT2(0.667f, 0.45f);


	vertices[4].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[4].uv = DirectX::XMFLOAT2(0.667f, 0.0f);

	vertices[5].position = DirectX::XMFLOAT3(-0.5f + edgeScaleX, 0.5f, 0.0f);
	vertices[5].uv = DirectX::XMFLOAT2(0.733f, 0.0f);

	vertices[6].position = DirectX::XMFLOAT3(-0.5f + edgeScaleX, 0.5f - edgeScaleY, 0.0f);
	vertices[6].uv = DirectX::XMFLOAT2(0.733f, 0.05f);

	vertices[7].position = DirectX::XMFLOAT3(-0.5f, 0.5f - edgeScaleY, 0.0f);
	vertices[7].uv = DirectX::XMFLOAT2(0.667f, 0.05f);


	vertices[8].position = DirectX::XMFLOAT3(0.5f - edgeScaleX, 0.5f, 0.0f);
	vertices[8].uv = DirectX::XMFLOAT2(0.933f, 0.0f);

	vertices[9].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[9].uv = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[10].position = DirectX::XMFLOAT3(0.5f, 0.5f - edgeScaleY, 0.0f);
	vertices[10].uv = DirectX::XMFLOAT2(1.0f, 0.05f);

	vertices[11].position = DirectX::XMFLOAT3(0.5f - edgeScaleX, 0.5f - edgeScaleY, 0.0f);
	vertices[11].uv = DirectX::XMFLOAT2(0.933f, 0.05f);


	vertices[12].position = DirectX::XMFLOAT3(0.5f - edgeScaleX, -0.5f + edgeScaleY, 0.0f);
	vertices[12].uv = DirectX::XMFLOAT2(0.933f, 0.45f);

	vertices[13].position = DirectX::XMFLOAT3(0.5f, -0.5f + edgeScaleY, 0.0f);
	vertices[13].uv = DirectX::XMFLOAT2(1.0f, 0.45f);

	vertices[14].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[14].uv = DirectX::XMFLOAT2(1.0f, 0.5f);

	vertices[15].position = DirectX::XMFLOAT3(0.5f - edgeScaleX, -0.5f, 0.0f);
	vertices[15].uv = DirectX::XMFLOAT2(0.933f, 0.5f);


	vertices[16].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
	vertices[16].uv = DirectX::XMFLOAT2(0.0f, 1.0f);


	vertices[17].position = DirectX::XMFLOAT3(0.5f, 0.5f, -0.0f);
	vertices[17].uv = DirectX::XMFLOAT2(0.667f, 0.0f);

	vertices[18].position = DirectX::XMFLOAT3(0.5f, -0.5f, -0.0f);
	vertices[18].uv = DirectX::XMFLOAT2(0.667f, 1.0f);

	vertices[19].position = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.0f);
	vertices[19].uv = DirectX::XMFLOAT2(0.0f, 0.0f);
}

void UITextureRect::SetIndices()
{
	indices = { 16, 17, 18,
				16, 19, 17,
				0, 1, 2,
				0, 3, 1,
				3, 6, 1,
				6, 3, 7,
				5, 6, 7,
				4, 5, 7,
				5, 8, 6,
				6, 8, 11,
				8, 9, 10,
				8, 10, 11,
				10, 12, 11,
				13, 12, 10,
				12, 13, 14,
				12, 14, 15,
				2, 12, 15,
				2, 1, 12,};
}

UITextureRect::~UITextureRect()
{
	SafeRelease(&srv);
}

void UITextureRect::Update()
{
	UpdateWorld();
	Move();
}

void UITextureRect::Move()
{

}

void UITextureRect::GUI()
{
}

std::string UITextureRect::MakeLabel(std::string preFix)
{
	return std::string();
}

void UITextureRect::MapVertexBuffer()
{
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void UITextureRect::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);

}
