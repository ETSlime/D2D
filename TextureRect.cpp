#include "TextureRect.h"


TextureRect::TextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, bool isCenter)
	:RectBase(position, size, rotation), centered(isCenter)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"VertexTexture.hlsl");

	shaderBuffer = new UploadBuffer<ShaderData>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
	shaderData.enabled = 1.0f;
	shaderData.fadeIn = 0.0f;
	shaderData.fadeOut = 0.0f;
	shaderData.totalTime = 0.0f;
	shaderData.startTime = 0.0f;

}

void TextureRect::SetVertices()
{
	vertices.assign(4, VertexTexture());

	vertices[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0, 1);


	vertices[1].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(1, 0);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(1, 1);

	vertices[3].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[3].uv = DirectX::XMFLOAT2(0, 0);

	//vertices[0].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	//vertices[0].uv = DirectX::XMFLOAT2(0, 1);


	//vertices[1].position = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	//vertices[1].uv = DirectX::XMFLOAT2(1, 0);

	//vertices[2].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	//vertices[2].uv = DirectX::XMFLOAT2(1, 1);

	//vertices[3].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//vertices[3].uv = DirectX::XMFLOAT2(0, 0);

	if (centered)
	{
		for (auto&v : vertices)
			v.position -= DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	}
}

void TextureRect::SetIndices()
{
	indices = { 0, 1, 2, 0, 3, 1 };
	//indices = { 3, 1, 2, 3, 2, 0 };
}


TextureRect::~TextureRect()
{
	SAFE_DELETE(boundingBox);
}


void TextureRect::Update()
{
	shaderData.totalTime = Timer::TotalTime();
	shaderBuffer->MapData(mDeviceContext, shaderData);
	UpdateWorld();
	Move();
}

void TextureRect::Render()
{
	mDeviceContext->PSSetConstantBuffers(2, 1, shaderBuffer->Resource());
	__super::Render();
}

void TextureRect::Move()
{

}

void TextureRect::GUI()
{
}

std::string TextureRect::MakeLabel(std::string preFix)
{
	return std::string();
}

void TextureRect::MapVertexBuffer()
{
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TextureRect::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);

}
