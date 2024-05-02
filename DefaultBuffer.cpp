#include "DefaultBuffer.h"

DefaultBuffer::DefaultBuffer(){}

void DefaultBuffer::CreateDefaultBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	const void* initData, UINT count, UINT byteSize, const D3D11_USAGE& usage)
{
	stride = byteSize;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = byteSize * count;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

	subData.pSysMem = initData;

	HRESULT hr = device->CreateBuffer(&desc, &subData, &mDefaultBuffer);
	assert(SUCCEEDED(hr));
}

DefaultBuffer::~DefaultBuffer()
{
	SafeRelease(&mDefaultBuffer);
}

//void IndexBuffer::SetIA()
//{
//	DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, offset);
//}
