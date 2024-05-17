#include "IShader.h"

void IShader::CompileShader(std::wstring path, std::string entryName, std::string profile, ID3DBlob** blob)
{
	ID3DBlob* error = nullptr;
	HRESULT  hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		blob,
		&error
	);
	CheckShaderError(hr, error);
	SafeRelease(&error);
}

//MessageBoxA(::handle, str.c_str(), "Shader Error", MB_OK);
void IShader::CheckShaderError(HRESULT hr, ID3DBlob* error)
{
	if (FAILED(hr))
	{
		if (error)
		{
			std::string str = (const char*)error->GetBufferPointer();
		}
		assert(SUCCEEDED(false));
	}
}
