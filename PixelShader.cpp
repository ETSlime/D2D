#include "Util.h"
#include "IShader.h"
#include "PixelShader.h"

PixelShader::~PixelShader()
{
	Clear();
}

PixelShader::PixelShader(ID3D11Device* device,ID3D11DeviceContext* deviceContext)
{
	mDevice = device;
	mDeviceContext = deviceContext;
}

void PixelShader::Create(const std::wstring path, const std::string entryName)
{
	this->path = path;
	this->entryName = entryName;

	CompileShader(path, entryName, "ps_5_0", &blob);
	HRESULT hr = mDevice->CreatePixelShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	Assert(SUCCEEDED(hr));
}

void PixelShader::Clear()
{
	SafeRelease(&blob);
	SafeRelease(&shader);
}

void PixelShader::SetShader()
{
	mDeviceContext->PSSetShader(shader, nullptr, 0);
}
