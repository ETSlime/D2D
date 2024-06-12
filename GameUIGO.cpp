#include "Util.h"
#include "GameUIGO.h"
#include "TextureRect.h"


GameUIGO::GameUIGO(const D2DResource* D2DResource, const WinSize* winSize, GameUI::UIRenderMode mode)
	:mD2DResource(D2DResource), curWinSize(winSize), UImode(mode){}

void GameUIGO::Init()
{
	gameUI = new GameUI(mD2DResource, curWinSize, UImode);
}

void GameUIGO::Destroy()
{
	SAFE_DELETE(gameUI);
}

void GameUIGO::Update()
{
	gameUI->Update();
}

void GameUIGO::Render()
{
	gameUI->Render();
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

void GameUIGO::ChangeUIMode(GameUI::UIRenderMode mode)
{
	UImode = mode; 
	if (gameUI)
		gameUI->ChangeRenderMode(UImode);
	else
		gameUI = new GameUI(mD2DResource, curWinSize, UImode);
}