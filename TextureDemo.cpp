#include "Util.h"
#include "TextureDemo.h"
#include "TextureRect.h"
#include "UI.h"


TextureDemo::TextureDemo(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:mDevice(device), mDeviceContext(deviceContext){}

void TextureDemo::Init()
{
	tr = new TextureRect(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 360, 0.1), DirectX::XMFLOAT3(WinMaxWidth, WinMaxHeight, 1), 0.0f, TexturePath + L"background.png");
	startUI = new UI(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 250, 0), DirectX::XMFLOAT3(400, 100, 1), 0);
}

void TextureDemo::Destroy()
{
	SAFE_DELETE(tr);

}

void TextureDemo::Update()
{
	tr->Update();
	//startUI->UpdateStart();



	if (startUI->GetPressWhat() == 1)
	{
		startUI->SetPressWhat(0);
		Valid = false;
	}

}

void TextureDemo::Render()
{
	tr->Render();
	//startUI->RenderStart();

}

void TextureDemo::PostRender()
{
}

void TextureDemo::GUI()
{
}

bool TextureDemo::IsValid()
{
	return Valid;
}

void TextureDemo::SetIsValid(bool _valid)
{
	this->Valid = _valid;
}
