#include "GameUI.h"


GameUI::GameUI(const D2DResource* D2DResource, const WinSize* winSize, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, UIMode mode)
	:mD2DResource(D2DResource), curWinSize(winSize), position(position), size(size), mode(mode)
{
	startSelector.baseX = 0.48;
	startSelector.baseY = 0.435;
	startSelector.position = DirectX::XMFLOAT3(startSelector.baseX * curWinSize->width, startSelector.baseY * curWinSize->height, 0.0f);
	startSelector.size = DirectX::XMFLOAT3(250, 50, 1.0f);
	startSelector.curIdx = 0;
	startSelector.textureRect = new SelectorTextureRect(startSelector.position, 
		startSelector.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	switch (mode)
	{
	case StartMenu:
		this->init_startUI();
		break;
	}
}

GameUI::~GameUI(){}



void GameUI::init_startUI()
{
	startButtons.push_back(new Button(L"始めから", DirectX::XMFLOAT3(0.4, 0.535, 0), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startGame, true));
	startButtons.push_back(new Button(L"続きから", DirectX::XMFLOAT3(0.4, 0.635, 0), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	startButtons.push_back(new Button(L"ゲーム終了", DirectX::XMFLOAT3(0.4, 0.735, 0), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	startButtons.push_back(new Button(L"遊ぶ方法説明", DirectX::XMFLOAT3(0.4, 0.835, 0), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));


	base = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	
}



void GameUI::Render()
{
	switch (mode)
	{
	case StartMenu:
		for (int i = 0; i < startButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				startButtons[i]->text.c_str(),
				startButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				startButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		base->Render();
		startSelector.textureRect->Render();
		break;
	}	
}

void GameUI::Update()
{
	switch (mode)
	{
	case StartMenu:
		curTime = Timer::TotalTime();
		if (startSelector.isPressed)
		{
			// wait for key up
			if (Keyboard::get_instance().Up(VK_UP))
			{
				startSelector.MoveUp(startButtons);
				startSelector.isPressed = false;
			}
			else if (Keyboard::get_instance().Up(VK_DOWN))
			{
				startSelector.MoveDown(startButtons);
				startSelector.isPressed = false;
			}

			// if still pressed, move selector
			curTime = Timer::TotalTime();
			if (curTime - lastTime > startSelector.selectorMoveTimeInterval)
			{
				lastTime = curTime;
				if (startSelector.moveDirection == Selector::UP)
					startSelector.MoveUp(startButtons);
				else
					startSelector.MoveDown(startButtons);
			}
		}
		else if (Keyboard::get_instance().Press(VK_UP))
		{
			startSelector.moveDirection = Selector::UP;
			startSelector.isPressed = true;
			lastTime = curTime;
		}
		else if (Keyboard::get_instance().Press(VK_DOWN))
		{
			startSelector.moveDirection = Selector::DOWN;
			startSelector.isPressed = true;
			lastTime = curTime;
		}
		startSelector.textureRect->Update();

		for (int i = 0; i < startButtons.size(); i++)
		{
			startButtons[i]->UpdateTextRect(curWinSize);

		}
		base->Update();

		if (Keyboard::get_instance().Down('C') ||
			Keyboard::get_instance().Down(VK_RETURN) ||
			Keyboard::get_instance().Down(VK_SPACE))
		{
			startSelector.Execute(startButtons);
		}

		break;
	}
}