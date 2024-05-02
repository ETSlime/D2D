#pragma once

#include "IShader.h"

class PixelShader : public IShader
{
public:

	~PixelShader();
	PixelShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void Create(const std::wstring path, const std::string entryName) override;
	virtual void Clear() override;
	virtual void SetShader() override;

	ID3DBlob* GetBlob() { return blob; }
	ID3D11PixelShader* GetResource() { return shader; }

private:
	ID3D11PixelShader* shader = nullptr;
	ID3DBlob* blob = nullptr;
};