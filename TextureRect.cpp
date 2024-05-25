#include "TextureRect.h"
//#include <D3DX11tex.h>


TextureRect::TextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, bool isCenter)
	:RectBase(position, size, rotation), centered(isCenter)
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"VertexTexture.hlsl");

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

	if (centered)
	{
		for (auto&v : vertices)
			v.position -= DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	}
}

void TextureRect::SetIndices()
{
	indices = { 0, 1, 2, 0, 3, 1 };
}


TextureRect::~TextureRect()
{
	SAFE_DELETE(mVertexBuffer);
	SAFE_DELETE(mIndexBuffer);
	SAFE_DELETE(mVertexShader);
	SAFE_DELETE(mPixelShader);
	SAFE_DELETE(mInputLayout);
	SAFE_DELETE(mWorldBuffer);
	SafeRelease(&srv);

	SAFE_DELETE(boundingBox);
}


void TextureRect::Update()
{
	UpdateWorld();
	Move();
}

//void TextureRect::Render()
//{
//	//mVertexBuffer->SetIA();
//	//mIndexBuffer->SetIA();
//	UINT offset = 0;
//	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
//	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);
//	//mInputLayout->SetIA();
//	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());
//
//	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	mVertexShader->SetShader();
//	mPixelShader->SetShader();
//
//	//mWorldBuffer->SetVSBuffer(0);
//	mWorldBuffer->MapData(mDeviceContext, world);
//	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());
//
//	mDeviceContext->PSSetShaderResources(0, 1, &srv);
//
//	mDeviceContext->DrawIndexed(indices.size(), 0, 0);
//}

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
