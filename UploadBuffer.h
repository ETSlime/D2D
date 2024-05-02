#pragma once

#include "Util.h"

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT elementCount, 
        D3D11_BIND_FLAG bindFlags, const D3D11_USAGE& usage) :
        mElementByteSize(sizeof(T))
    {

        desc.Usage = usage;
        desc.ByteWidth = mElementByteSize * elementCount;
        desc.BindFlags = bindFlags;

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


        HRESULT hr = device->CreateBuffer(&desc, nullptr, &mUploadBuffer);

        assert(SUCCEEDED(hr));

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

    ~UploadBuffer()
    {
        SafeRelease(&mUploadBuffer);
    }

    ID3D11Buffer* const* Resource()const
    {
        return &mUploadBuffer;
    }

    
    void MapData(ID3D11DeviceContext* deviceContext, const T& data)
    {
        D3D11_MAPPED_SUBRESOURCE subResource;
        HRESULT hr = deviceContext->Map
        (
            mUploadBuffer, 0, D3D11_MAP_WRITE_DISCARD,
            0, &subResource
        );
        memcpy(subResource.pData, &data, mElementByteSize);
        deviceContext->Unmap(mUploadBuffer, 0);
    }

private:
    ID3D11Buffer* mUploadBuffer = nullptr;
    void* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;

    D3D11_BUFFER_DESC desc = { 0 };


};