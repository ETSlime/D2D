#include "Util.h"
#include "StartMenuGO.h"
#include "TextureRect.h"
#include "GameUI.h"


StartMenuGO::StartMenuGO(const D2DResource* D2DResource, const WinSize* WinSize)
	:mD2DResource(D2DResource), curWinSize(WinSize){}

void StartMenuGO::Init()
{
	startUI = new GameUI(mD2DResource, curWinSize, DirectX::XMFLOAT3(640, 200, 0), DirectX::XMFLOAT3(400, 300, 1), GameUI::StartMenu);
}

void StartMenuGO::Destroy()
{
	SAFE_DELETE(startUI);

}

void StartMenuGO::Update()
{
	startUI->Update();
}

void StartMenuGO::Render()
{
	startUI->Render();
}

void StartMenuGO::PostRender()
{
}

void StartMenuGO::GUI()
{
}

bool StartMenuGO::IsValid()
{
	return valid;
}

bool StartMenuGO::IsDestroyed()
{
	return destroy;
}

void StartMenuGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void StartMenuGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}