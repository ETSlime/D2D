#include "GameUI.h"

GameUI::GameUI(const D2DResource* D2DResource, const WinSize* winSize, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, UIMode mode)
	:mD2DResource(D2DResource), curWinSize(winSize), position(position), size(size), mode(mode)
{


	switch (mode)
	{
	case StartMenu:
		this->init_startUI();
		break;
	case MainMenu:
		this->init_mainMenu();
		break;
	}
}

GameUI::~GameUI(){}



void GameUI::init_startUI()
{
	startCursor.baseX = 0.48f;
	startCursor.baseY = 0.435f;
	startCursor.position = DirectX::XMFLOAT3(startCursor.baseX * curWinSize->width, startCursor.baseY * curWinSize->height, 0.0f);
	startCursor.size = DirectX::XMFLOAT3(250, 50, 1.0f);
	startCursor.curIdx = 0;
	startCursor.textureRect = new CursorTextureRect(startCursor.position,
		startCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	startButtons.push_back(new Button(L"始めから", DirectX::XMFLOAT3(0.4f, 0.535f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startGame, true));
	startButtons.push_back(new Button(L"続きから", DirectX::XMFLOAT3(0.4f, 0.635f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	startButtons.push_back(new Button(L"ゲーム終了", DirectX::XMFLOAT3(0.4f, 0.735f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	startButtons.push_back(new Button(L"遊ぶ方法説明", DirectX::XMFLOAT3(0.4f, 0.835f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));

	base = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
}

void GameUI::init_mainMenu()
{
	startCursor.baseX = 0.48f;
	startCursor.baseY = 0.435f;
	startCursor.position = DirectX::XMFLOAT3(startCursor.baseX * curWinSize->width, startCursor.baseY * curWinSize->height, 0.0f);
	startCursor.size = DirectX::XMFLOAT3(250, 50, 1.0f);
	startCursor.curIdx = 0;
	startCursor.textureRect = new CursorTextureRect(startCursor.position,
		startCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	startButtons.push_back(new Button(L"始めから", DirectX::XMFLOAT3(0.4f, 0.535f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startGame, true));
	startButtons.push_back(new Button(L"続きから", DirectX::XMFLOAT3(0.4f, 0.635f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	startButtons.push_back(new Button(L"ゲーム終了", DirectX::XMFLOAT3(0.4f, 0.735f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	startButtons.push_back(new Button(L"遊ぶ方法説明", DirectX::XMFLOAT3(0.4f, 0.835f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));

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
		startCursor.textureRect->Render();
		break;
	}	
}

void GameUI::Update()
{
	switch (mode)
	{
	case StartMenu:
		curTime = Timer::TotalTime();
		if (startCursor.isPressed)
		{
			// wait for key up
			if (Keyboard::get_instance().Up(VK_UP))
			{
				startCursor.MoveUp(startButtons);
				startCursor.isPressed = false;
			}
			else if (Keyboard::get_instance().Up(VK_DOWN))
			{
				startCursor.MoveDown(startButtons);
				startCursor.isPressed = false;
			}

			// if still pressed, move cursor
			curTime = Timer::TotalTime();
			if (curTime - lastTime > startCursor.cursorMoveTimeInterval)
			{
				lastTime = curTime;
				if (startCursor.moveDirection == Cursor::UP)
					startCursor.MoveUp(startButtons);
				else
					startCursor.MoveDown(startButtons);
			}
		}
		else if (Keyboard::get_instance().Press(VK_UP))
		{
			startCursor.moveDirection = Cursor::UP;
			startCursor.isPressed = true;
			lastTime = curTime;
		}
		else if (Keyboard::get_instance().Press(VK_DOWN))
		{
			startCursor.moveDirection = Cursor::DOWN;
			startCursor.isPressed = true;
			lastTime = curTime;
		}
		startCursor.textureRect->Update();

		for (int i = 0; i < startButtons.size(); i++)
		{
			startButtons[i]->UpdateTextRect(curWinSize);

		}
		base->Update();

		if (Keyboard::get_instance().Down('C') ||
			Keyboard::get_instance().Down(VK_RETURN) ||
			Keyboard::get_instance().Down(VK_SPACE))
		{
			startCursor.Execute(startButtons);
		}

		break;
	}
}