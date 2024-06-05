#include "Util.h"
#include "GameUIGO.h"
#include "TextureRect.h"


GameUIGO::GameUIGO(const D2DResource* D2DResource, const WinSize* winSize, GameUI::UIMode mode)
	:mD2DResource(D2DResource), curWinSize(winSize), UImode(mode){}

void GameUIGO::Init()
{
	startUI = new GameUI(mD2DResource, curWinSize, DirectX::XMFLOAT3(640, 200, 0), DirectX::XMFLOAT3(400, 300, 1), UImode);
}

void GameUIGO::Destroy()
{
	SAFE_DELETE(startUI);
}

void GameUIGO::Update()
{
	startUI->Update();
}

void GameUIGO::Render()
{
	startUI->Render();
}

void GameUIGO::PostRender()
{
}

void GameUIGO::GUI()
{
}

bool GameUIGO::IsValid()
{
	return valid;
}

bool GameUIGO::IsDestroyed()
{
	return destroy;
}

void GameUIGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void GameUIGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}