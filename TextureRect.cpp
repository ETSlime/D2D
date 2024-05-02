#include "TextureRect.h"
#include <D3DX11tex.h>


TextureRect::TextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring path)
	:mDevice(device), mDeviceContext(deviceContext),position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
		vertices[0].uv = DirectX::XMFLOAT2(0, 1);


		vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
		vertices[1].uv = DirectX::XMFLOAT2(1, 0);

		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[2].uv = DirectX::XMFLOAT2(1, 1);

		vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
		vertices[3].uv = DirectX::XMFLOAT2(0, 0);
	}

	//VertexBuffer
	{
		mVertexBuffer = new DefaultBuffer();
		mVertexBuffer->CreateDefaultBuffer(device, deviceContext, vertices.data(), 
			vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);


	}

	//IndexBuffer
	{
		indices = { 0, 1, 2, 0, 3, 1 };

		mIndexBuffer = new DefaultBuffer();
		mIndexBuffer->CreateDefaultBuffer(device, deviceContext, indices.data(),
			indices.size(), sizeof(UINT), D3D11_USAGE_IMMUTABLE);
	}

	//VertexShader
	{
		mVertexShader = new VertexShader(mDevice, mDeviceContext);
		mVertexShader->Create(ShaderPath + L"VertexTexture.hlsl", "VS");
	}

	//PixelShader
	{
		mPixelShader = new PixelShader(mDevice, mDeviceContext);
		mPixelShader->Create(ShaderPath + L"VertexTexture.hlsl", "PS");
	}

	//InputLayout
	{
		mInputLayout = new InputLayout(mDevice, mDeviceContext);
		mInputLayout->Create(VertexTexture::descs, VertexTexture::count, mVertexShader->GetBlob());
	}

	//WorldBuffer
	{
		mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
			device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
	}

	//SRV
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			device,
			path.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		Assert(SUCCEEDED(hr));
	}
}

TextureRect::TextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
	:mDevice(device), mDeviceContext(deviceContext), position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
		vertices[0].uv = DirectX::XMFLOAT2(0, 1);

		vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
		vertices[1].uv = DirectX::XMFLOAT2(1, 0);

		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[2].uv = DirectX::XMFLOAT2(1, 1);

		vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
		vertices[3].uv = DirectX::XMFLOAT2(0, 0);
	}

	//VertexBuffer
	{
		mVertexBuffer = new DefaultBuffer();
		mVertexBuffer->CreateDefaultBuffer(mDevice, mDeviceContext, vertices.data(), 
			vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);
	}

	//IndexBuffer
	{
		indices = { 0, 1, 2, 0, 3, 1 };

		mIndexBuffer = new DefaultBuffer();
		mIndexBuffer->CreateDefaultBuffer(mDevice, mDeviceContext, indices.data(), 
			indices.size(), sizeof(UINT), D3D11_USAGE_IMMUTABLE);
	}

	//VertexShader
	{
		mVertexShader = new VertexShader(mDevice, mDeviceContext);
		mVertexShader->Create(ShaderPath + L"VertexTexture.hlsl", "VS");
	}

	//PixelShader
	{
		mPixelShader = new PixelShader(mDevice, mDeviceContext);
		mPixelShader->Create(ShaderPath + L"VertexTexture.hlsl", "PS");
	}

	//InputLayout
	{
		mInputLayout = new InputLayout(mDevice, mDeviceContext);
		mInputLayout->Create(VertexTexture::descs, VertexTexture::count, mVertexShader->GetBlob());
	}


	edge = new RectEdge();
	box = new BoundingBox(edge);
	mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
		device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
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

	SAFE_DELETE(edge);
	SAFE_DELETE(box);
}

void TextureRect::SetShader(std::wstring shaderPath)
{
	mVertexShader->Clear();
	mPixelShader->Clear();

	mVertexShader->Create(shaderPath, "VS");
	mPixelShader->Create(shaderPath, "PS");
}

void TextureRect::Update()
{
	UpdateWorld();
	Move();
}

void TextureRect::UpdateWorld()
{
	DirectX::XMMATRIX S, R, T, W;
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	R = DirectX::XMMatrixRotationZ((float)D3DXToRadian(rotation));

	W = S * R * T;
	XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));
	//mWorldBuffer->SetWorld(world);

	
	/*DirectX::XMStoreFloat3(&edge->LT, DirectX::XMVector3TransformCoord(XMLoadFloat3(&vertices[3].position), W));
	DirectX::XMStoreFloat3(&edge->RB, DirectX::XMVector3TransformCoord(XMLoadFloat3(&vertices[2].position), W));
	edge->LT = { edge->LT.x + CollisionEdge_LT.x,edge->LT.y - CollisionEdge_LT.y,edge->LT.z };
	edge->RB = { edge->RB.x - CollisionEdge_RB.x,edge->RB.y + CollisionEdge_RB.y,edge->RB.z };*/

}

void TextureRect::Render()
{
	//mVertexBuffer->SetIA();
	//mIndexBuffer->SetIA();
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);
	//mInputLayout->SetIA();
	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVertexShader->SetShader();
	mPixelShader->SetShader();

	//mWorldBuffer->SetVSBuffer(0);
	mWorldBuffer->MapData(mDeviceContext, world);
	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

	mDeviceContext->PSSetShaderResources(0, 1, &srv);

	mDeviceContext->DrawIndexed(indices.size(), 0, 0);
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
