#pragma once

#include "Util.h"

class DefaultBuffer
{
public:
	~DefaultBuffer();
	DefaultBuffer();
	void CreateDefaultBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		const void* initData, UINT count, UINT byteSize, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() { return mDefaultBuffer; }

	ID3D11Buffer* const* GetBuffer() { return &mDefaultBuffer; }

	const UINT* GetStride() { return &stride; }

	void SetIA();

private:
	ID3D11Buffer* mDefaultBuffer = nullptr;
	UINT stride = 0;
	UINT offset = 0;
};
