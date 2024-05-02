#pragma once
#include "Util.h"

class InputLayout
{
public:
	~InputLayout();
	InputLayout(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void Create(D3D11_INPUT_ELEMENT_DESC* descs, UINT cout, ID3DBlob* blob);

	void SetIA();

	ID3D11InputLayout* GetInputLayout() { return inputLayout; }

private:
	ID3D11InputLayout* inputLayout = nullptr;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};