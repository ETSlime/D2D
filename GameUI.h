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
		PLAYERSTATES,
		ITEMCHECK,
	};
public:

	GameUI(const D2DResource* D2DResource, const WinSize* winSize, UIRenderMode mode);
	~GameUI();



	void Update();
	void Render();


	void ChangeRenderMode(GameUI::UIRenderMode mode) { InitGameUI(mode); renderMode = mode; }
	void SetRenderModeOnChanging() { renderModeOnChanging = true; }

private:

	struct Cursor
	{
		~Cursor()
		{
			SAFE_DELETE(textureRect);
		}

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

		void MoveUp(std::vector<std::unique_ptr<Button>>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx > 0 ? curIdx - 1 : buttons.size() - 1;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void MoveUp(std::unordered_map<ItemCategory, Button*>& buttons)
		{

		}

		void MoveUp()
		{
			curIdx = curIdx > 0 ? curIdx - 1 : 0;
			Update(curIdx);
		}

		void MoveDown(std::vector<std::unique_ptr<Button>>& buttons)
		{
			buttons[curIdx]->isSelected = false;
			curIdx = curIdx < buttons.size() - 1 ? curIdx + 1 : 0;
			Update(curIdx);
			buttons[curIdx]->isSelected = true;
		}

		void MoveDown(std::unordered_map<ItemCategory, Button*>& buttons)
		{

		}

		void MoveDown(int maxCount)
		{
			curIdx = curIdx < maxCount ? curIdx + 1 : maxCount;
			Update(curIdx);
		}

		void Execute(std::vector<std::unique_ptr<Button>>& buttons)
		{
			if (buttons[curIdx]->pOnClick)
				buttons[curIdx]->pOnClick();
		}
	};

	// Init UI
	void InitGameUI(GameUI::UIRenderMode mode);
	void InitMainMenu();
	void InitInGameUI();
	void InitPlayerState();
	void InitItemCheck();

	
	// draw text
	IDWriteTextFormat* DynamicTextFormat(const std::wstring& text, const D2D1_RECT_F* textRect, bool* isTransformed = nullptr);
	void DrawTextWithSpacing(IDWriteTextFormat* pTextFormat, const std::wstring& text, const D2D1_RECT_F* rect);
	D2D1_RECT_F GetTextRect(float left, float top, float right, float bottom);
	std::wstring GetFormattedTime() const;

	// cursor & button
	Cursor startCursor, mainGameCursor_1st, mainGameCursor_2nd;
	float curTime = 0, lastTime = 0;
	std::vector<std::unique_ptr<Button>> startButtons, gameMenuButtons, itemCategoryButtons;
	std::unordered_set<ItemCategory> itemCategorySet;
	//std::unordered_map<ItemCategory, Button*> itemCategoryButtons;
	void UpdateCursorAndButton(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons);
	void UpdateMonsterCursor(Cursor& cursor);
	void UpdateItemCursor(Cursor& cursor, std::vector<Button*>& buttons);
	void UpdateUIState();

	// render
	void RenderItem();
	void RenderPlayerStates();
	void RenderMonsterStates();

	// start menu
	UITextureRect* base = nullptr;
	// in game menu
	UITextureRect* rightDispalyBase = nullptr;
	UITextureRect* leftButtonBase = nullptr;
	UITextureRect* timerBase = nullptr;
	UITextureRect* walkingStepBase = nullptr;
	// item check
	UITextureRect* itemCheckTitleBase = nullptr;
	UITextureRect* itemCheckDetailBase = nullptr;
	UITextureRect* itemCheckCategoryBase = nullptr;
	UITextureRect* itemCheckDescriptionBase = nullptr;
	// icon texture
	TextureRect* playerIcon = nullptr;
	TextureRect* yellowKeyIcon = nullptr;
	TextureRect* blueKeyIcon = nullptr;
	TextureRect* redKeyIcon = nullptr;
	TextureRect* greenKeyIcon = nullptr;

	// monster texture&data
	std::map<UINT, std::tuple<int, std::unique_ptr<TextureRect>, MonsterData>> monsters;

	// Render mode/UI states
	UIState UIstate = UIState::MenuLevel1;
	UIRenderMode renderMode;
	bool renderModeOnChanging = false;

	// instance & resource
	Keyboard& keyboard = Keyboard::get_instance();
	Map& map = Map::get_instance();
	MagicTowerApp& mApp = MagicTowerApp::get_instance();
	Mouse& mouse = Mouse::get_instance();
	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
	const D2DResource* mD2DResource = nullptr;
	const WinSize* curWinSize = nullptr;
};

