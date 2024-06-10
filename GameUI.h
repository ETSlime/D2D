#pragma once

#include "Util.h"
#include "UIRect.h"
#include "UIalphabet.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "UITextureRect.h"
#include "TextureRect.h"
#include "CursorTextureRect.h"
#include "ButtonOnClick.h"
#include "MagicTowerApp.h"
#include "Database.h"


enum class UIState 
{
	MenuLevel1,
	MenuLevel2
};

class GameUI
{
public:
	enum UIRenderMode
	{
		STARTMENU,
		INGAMEUI,
		PLAYERSTATES
	};
public:

	GameUI(const D2DResource* D2DResource, const WinSize* winSize, UIRenderMode mode);
	~GameUI();



	void Update();
	void Render();
	UIRect* GetanimRect() { return animRect; }

	void RenderPlayerStates();
	void RenderMonsterStates();

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
		bool enabled = true;
		float cursorMoveTimeInterval = 0.2f;
		float spacing = 0.1f;
		enum
		{
			UP,
			DOWN
		};

		void Update(short curIdx)
		{
			position.y = (baseY - curIdx * spacing) * WinMaxHeight;
			textureRect->UpdatePosition(position);
		}

		void MoveUp(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx > 0 ? curIdx - 1 : buttons.size() - 1;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void MoveUp()
		{
			curIdx = curIdx > 0 ? curIdx - 1 : 0;
			Update(curIdx);
		}

		void MoveDown(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx < buttons.size() - 1 ? curIdx + 1 : 0;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void MoveDown()
		{
			curIdx = curIdx < 6 ? curIdx + 1 : 6;
			Update(curIdx);
		}

		void Execute(std::vector<Button*>& buttons)
		{
			buttons[curIdx]->pOnClick();
		}
	};

	void InitMainMenu();
	void InitInGameUI();
	void InitPlayerState();

	void UpdateTextRect(D2D1_RECT_F* textRect);
	void UpdateCursorAndButton(Cursor& cursor, std::vector<Button*>& buttons);
	void UpdateMonsterCursor(Cursor& cursor);
	IDWriteTextFormat* DynamicTextFormat(const std::wstring& text, const D2D1_RECT_F* textRect, bool* isTransformed = nullptr);
	void DrawTextWithSpacing(IDWriteTextFormat* pTextFormat, const std::wstring& text, const D2D1_RECT_F* rect);

	Cursor startCursor, mainGameCursor_1st, mainGameCursor_2nd;
	float curTime = 0, lastTime = 0;
	std::vector<Button*> startButtons, gameMenuButtons;

	// texture
	UITextureRect* base = nullptr;
	UITextureRect* rightDispalyBase = nullptr;
	UITextureRect* leftButtonBase = nullptr;
	UITextureRect* timerBase = nullptr;
	UITextureRect* walkingStepBase = nullptr;
	// icon texture
	TextureRect* playerIcon = nullptr;
	TextureRect* yellowKeyIcon = nullptr;
	TextureRect* blueKeyIcon = nullptr;
	TextureRect* redKeyIcon = nullptr;
	TextureRect* greenKeyIcon = nullptr;

	// monster texture&data
	std::unordered_map<UINT, std::tuple<UINT, TextureRect*, MonsterData>> monsters;

	int mode = 0;  
	int pressWhat = 0;
	UIRect* animRect = nullptr;

	int time = 0;

	UIState UIstate = UIState::MenuLevel1;
	Keyboard& keyboard = Keyboard::get_instance();
	Map& map = Map::get_instance();
	MagicTowerApp& mApp = MagicTowerApp::get_instance();
	Mouse& mouse = Mouse::get_instance();
	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
	const D2DResource* mD2DResource = nullptr;
	const WinSize* curWinSize = nullptr;
};

