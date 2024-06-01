#pragma once

#include "RectBase.h"

// Explicit template instantiation for VertexTile
template class RectBase<VertexTile>;
template class RectBase<SimpleVertex>;

RectBase<VertexTexture>::RectBase(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
	:position(position), size(size), rotation(rotation){}

template<typename VertexType>
RectBase<VertexType>::RectBase(){}


void RectBase<VertexTexture>::CreateRenderResource(const std::vector<VertexTexture>vertices,
	const std::vector<UINT>indices, std::wstring shaderPath)
{
	__super::CreateRenderResource(vertices, indices, shaderPath);
}

template <typename VertexType>
void RectBase<VertexType>::CreateRenderResource(const std::vector<VertexType>vertices,
	const std::vector<UINT>indices, std::wstring shaderPath)
{
	__super::CreateRenderResource(vertices, indices, shaderPath);
}

template <typename VertexType>
void IRectBase::CreateRenderResource(const std::vector<VertexType>vertices,
	const std::vector<UINT>indices, std::wstring shaderPath)
{
	this->indexCount = indices.size();

	//VertexBuffer
	this->mVertexBuffer = new DefaultBuffer();
	this->mVertexBuffer->CreateDefaultBuffer(this->mDevice, this->mDeviceContext, vertices.data(),
		vertices.size(), sizeof(VertexType), D3D11_USAGE_DYNAMIC);

	//IndexBuffer
	this->mIndexBuffer = new DefaultBuffer();
	this->mIndexBuffer->CreateDefaultBuffer(this->mDevice, this->mDeviceContext, indices.data(),
		indices.size(), sizeof(UINT), D3D11_USAGE_DYNAMIC);

	//VertexShader
	this->mVertexShader = new VertexShader(this->mDevice, this->mDeviceContext);

	//PixelShader
	this->mPixelShader = new PixelShader(this->mDevice, this->mDeviceContext);

	SetShader(shaderPath);

	//InputLayout
	this->mInputLayout = new InputLayout(this->mDevice, this->mDeviceContext);
	this->mInputLayout->Create(VertexType::descs, VertexType::count, this->mVertexShader->GetBlob());

	//WorldBuffer
	this->mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
		this->mDevice, this->mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
}




template<typename VertexType>
RectBase<VertexType>::~RectBase()
{
	SAFE_DELETE(this->mVertexBuffer);
	SAFE_DELETE(this->mIndexBuffer);
	SAFE_DELETE(this->mVertexShader);
	SAFE_DELETE(this->mPixelShader);
	SAFE_DELETE(this->mInputLayout);
	SAFE_DELETE(this->mWorldBuffer);
}

RectBase<VertexTexture>::~RectBase()
{
	SAFE_DELETE(this->mVertexBuffer);
	SAFE_DELETE(this->mIndexBuffer);
	SAFE_DELETE(this->mVertexShader);
	SAFE_DELETE(this->mPixelShader);
	SAFE_DELETE(this->mInputLayout);
	SAFE_DELETE(this->mWorldBuffer);
	SAFE_DELETE(this->srv);
}

void IRectBase::SetShader(std::wstring shaderPath)
{
	this->mVertexShader->Clear();
	this->mPixelShader->Clear();

	this->mVertexShader->Create(shaderPath, "VS");
	this->mPixelShader->Create(shaderPath, "PS");
}

void RectBase<VertexTexture>::UpdateWorld()
{
	DirectX::XMMATRIX S, R, T, W;
	DirectX::XMFLOAT4X4 world;
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	R = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));

	W = S * R * T;

	XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));

	mWorldBuffer->MapData(mDeviceContext, world);
}

void RectBase<VertexTexture>::UpdatePosition(DirectX::XMFLOAT3 newPosition)
{
	position = newPosition;
}

void RectBase<VertexTexture>::UpdateSize(DirectX::XMFLOAT3 newSize)
{
	size = newSize;
}

void RectBase<VertexTexture>::UpdateRotation(float newRotation)
{
	rotation = newRotation;
}


void RectBase<VertexTexture>::Render()
{
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);

	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVertexShader->SetShader();
	mPixelShader->SetShader();

	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

	mDeviceContext->PSSetShaderResources(0, 1, &srv);

	mDeviceContext->DrawIndexed(indexCount, 0, 0);
}

template<typename VertexType>
void RectBase<VertexType>::Render()
{
	// Only set vertex & index buffer and shaders
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);

	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	mVertexShader->SetShader();
	mPixelShader->SetShader();

	
}