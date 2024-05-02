#include "Util.h"
#include "IShader.h"
#include "VertexShader.h"

VertexShader::~VertexShader()
{
	Clear();
}

VertexShader::VertexShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	mDevice = device;
	mDeviceContext = deviceContext;
}

void VertexShader::Create(const std::wstring path, const std::string entryName)
{
	this->path = path;
	this->entryName = entryName;

	CompileShader(path, entryName, "vs_5_0", &blob);
	try 
	{
		ThrowIfFailed(mDevice->CreateVertexShader
		(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&shader
		));
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"Shader Load Failed", MB_OK);
	}

	
}

void VertexShader::Clear()
{
	SafeRelease(&blob);
	SafeRelease(&shader);
}

void VertexShader::SetShader()
{
	mDeviceContext->VSSetShader(shader, nullptr, 0);
}
