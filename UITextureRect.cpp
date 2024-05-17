#include "UITextureRect.h"

UITextureRect::UITextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring texturePath)
	:RectBase(position, size, rotation)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"UITexture.hlsl");

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

//UITextureRect::UITextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
//	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
//	:mDevice(device), mDeviceContext(deviceContext), position(position), size(size), rotation(rotation)
//{
//	//vertices
//	{
//		vertices.assign(4, VertexTexture());
//
//		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
//		vertices[0].uv = DirectX::XMFLOAT2(0, 1);
//
//		vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
//		vertices[1].uv = DirectX::XMFLOAT2(1, 0);
//
//		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
//		vertices[2].uv = DirectX::XMFLOAT2(1, 1);
//
//		vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
//		vertices[3].uv = DirectX::XMFLOAT2(0, 0);
//	}
//
//	//VertexBuffer
//	mVertexBuffer = new DefaultBuffer();
//	mVertexBuffer->CreateDefaultBuffer(device, deviceContext, vertices.data(),
//		vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);
//
//	//IndexBuffer
//	indices = { 0, 1, 2, 0, 3, 1 };
//	mIndexBuffer = new DefaultBuffer();
//	mIndexBuffer->CreateDefaultBuffer(device, deviceContext, indices.data(),
//		indices.size(), sizeof(UINT), D3D11_USAGE_IMMUTABLE);
//
//	//VertexShader
//	mVertexShader = new VertexShader(mDevice, mDeviceContext);
//	mVertexShader->Create(ShaderPath + L"UITexture.hlsl", "VS");
//
//	//PixelShader
//	mPixelShader = new PixelShader(mDevice, mDeviceContext);
//	mPixelShader->Create(ShaderPath + L"UITexture.hlsl", "PS");
//
//	//InputLayout
//	mInputLayout = new InputLayout(mDevice, mDeviceContext);
//	mInputLayout->Create(VertexTexture::descs, VertexTexture::count, mVertexShader->GetBlob());
//
//	//WorldBuffer
//	mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
//		device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
//
//}

void UITextureRect::SetVertices()
{
	float edgeSizeX = 25;
	float edgeSizeY = 10;
	float edgeScaleX = edgeSizeX / size.x;
	float edgeScaleY = edgeSizeY / size.y;

	vertices.assign(20, VertexTexture());

	vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 1.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0.667, 0.5);

	vertices[1].position = DirectX::XMFLOAT3(-0.5 + edgeScaleX, -0.5 + edgeScaleY, 0.0f);//DirectX::XMFLOAT3(-0.4f, -0.4f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(0.733, 0.45);

	vertices[2].position = DirectX::XMFLOAT3(-0.5 + edgeScaleX, -0.5f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(0.733, 0.5);

	vertices[3].position = DirectX::XMFLOAT3(-0.5f, -0.5 + edgeScaleY, 0.0f);
	vertices[3].uv = DirectX::XMFLOAT2(0.667, 0.45);


	vertices[4].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[4].uv = DirectX::XMFLOAT2(0.667, 0);

	vertices[5].position = DirectX::XMFLOAT3(-0.5 + edgeScaleX, 0.5f, 0.0f);
	vertices[5].uv = DirectX::XMFLOAT2(0.733, 0);

	vertices[6].position = DirectX::XMFLOAT3(-0.5 + edgeScaleX, 0.5 - edgeScaleY, 0.0f);
	vertices[6].uv = DirectX::XMFLOAT2(0.733, 0.05);

	vertices[7].position = DirectX::XMFLOAT3(-0.5f, 0.5 - edgeScaleY, 0.0f);
	vertices[7].uv = DirectX::XMFLOAT2(0.667, 0.05);


	vertices[8].position = DirectX::XMFLOAT3(0.5 - edgeScaleX, 0.5f, 0.0f);
	vertices[8].uv = DirectX::XMFLOAT2(0.933, 0);

	vertices[9].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[9].uv = DirectX::XMFLOAT2(1, 0);

	vertices[10].position = DirectX::XMFLOAT3(0.5f, 0.5 - edgeScaleY, 0.0f);
	vertices[10].uv = DirectX::XMFLOAT2(1, 0.05);

	vertices[11].position = DirectX::XMFLOAT3(0.5 - edgeScaleX, 0.5 - edgeScaleY, 0.0f);
	vertices[11].uv = DirectX::XMFLOAT2(0.933, 0.05);


	vertices[12].position = DirectX::XMFLOAT3(0.5 - edgeScaleX, -0.5 + edgeScaleY, 0.0f);
	vertices[12].uv = DirectX::XMFLOAT2(0.933, 0.45);

	vertices[13].position = DirectX::XMFLOAT3(0.5f, -0.5 + edgeScaleY, 0.0f);
	vertices[13].uv = DirectX::XMFLOAT2(1, 0.45);

	vertices[14].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[14].uv = DirectX::XMFLOAT2(1, 0.5);

	vertices[15].position = DirectX::XMFLOAT3(0.5 - edgeScaleX, -0.5f, 0.0f);
	vertices[15].uv = DirectX::XMFLOAT2(0.933, 0.5);


	vertices[16].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
	vertices[16].uv = DirectX::XMFLOAT2(0, 1);


	vertices[17].position = DirectX::XMFLOAT3(0.5f, 0.5f, -0.0f);
	vertices[17].uv = DirectX::XMFLOAT2(0.667, 0);

	vertices[18].position = DirectX::XMFLOAT3(0.5f, -0.5f, -0.0f);
	vertices[18].uv = DirectX::XMFLOAT2(0.667, 1);

	vertices[19].position = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.0f);
	vertices[19].uv = DirectX::XMFLOAT2(0, 0);
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
