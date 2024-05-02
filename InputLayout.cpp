#include "InputLayout.h"

InputLayout::~InputLayout()
{
	SafeRelease(&inputLayout);
}

InputLayout::InputLayout(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:mDevice(device), mDeviceContext(deviceContext)
{
}

void InputLayout::Create(D3D11_INPUT_ELEMENT_DESC* descs, UINT count, ID3DBlob* blob)
{
	if (!descs || !count || !blob)
		Assert(SUCCEEDED(false));

	HRESULT hr = mDevice->CreateInputLayout
	(
		descs, count,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout
	);
}

void InputLayout::SetIA()
{
	mDeviceContext->IASetInputLayout(inputLayout);
}


