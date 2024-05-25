#pragma once

#include "Util.h"
#include "UIRect.h"
#include "UIalphabet.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "UITextureRect.h"
#include "TextureRect.h"
#include "CursorTextureRect.h"
#include "Button.h"
#include "MagicTowerApp.h"

class GameUI
{
public:
	enum UIMode
	{
		StartMenu,
		MainMenu
	};
public:

	GameUI(const D2DResource* D2DResource, const WinSize* winSize, 
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, UIMode mode);
	~GameUI();

	void init_startUI();



	void Update();
	void Render();
	UIRect* GetanimRect() { return animRect; }


	int GetPressWhat() { return pressWhat; }
	void SetPressWhat(int a) { this->pressWhat = a; }



private:

	struct Cursor
	{
		CursorTextureRect* textureRect = nullptr;
		DirectX::XMFLOAT3 position = {};
		DirectX::XMFLOAT3 size = {};
		float baseX = 0;
		float baseY = 0;
		int curIdx = 0;
		bool moveDirection = 0;
		bool isPressed = false;
		float cursorMoveTimeInterval = 0.35f;

		enum
		{
			UP,
			DOWN
		};

		void Update(short curIdx)
		{
			position.y = (baseY - curIdx * 0.1f) * WinMaxHeight;
			textureRect->UpdatePosition(position);
		}

		void MoveUp(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx > 0 ? curIdx - 1 : buttons.size() - 1;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void MoveDown(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx < buttons.size() - 1 ? curIdx + 1 : 0;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void Execute(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->pOnClick();
		}
	};

	Cursor startCursor;
	float curTime = 0, lastTime = 0;
	std::vector<Button*> startButtons;
	UITextureRect* base = nullptr;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	int mode = 0;  
	int pressWhat = 0;
	UIRect* animRect = nullptr;

	int time = 0;

	Keyboard& keyboard = Keyboard::get_instance();
	Mouse& mouse = Mouse::get_instance();
	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
	const D2DResource* mD2DResource = nullptr;
	const WinSize* curWinSize = nullptr;
};

