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

#define GENERAL_BUTTON_SPACING		(0.075f)
#define MAX_MONSTER_STATE_COUNT		(6)
#define MONSTER_ICON_SPACING		(100)
#define MONSTER_STATUS_SPACING		(0.1333f)
#define SAVE_SLOTS_PER_PAGE			(4)
#define SAVE_SLOTS_SPACING			(0.217f)
#define SAVE_SLOT_SIZE				(12)
#define DIALOGUE_RECT_LEFT			(0.27f)
#define DIALOGUE_NAME_RECT_TOP		(0.55f)
#define DIALOGUE_RECT_RIGHT			(0.99f)
#define DIALOGUE_RECT_NAME_RIGHT	(0.55f)
#define DIALOGUE_NAME_RECT_BOTTOM	(0.67f)
#define DIALOGUE_ITEM_RECT_TOP		(0.37f)
#define DIALOGUE_ITEM_RECT_BOTTOM	(0.62f)
#define DIALOGUE_RECT_TOP			(0.64f)
#define DIALOGUE_RECT_BOTTOM		(0.82f)

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
		SAVEDATA,
		LOADDATA,
		DIALOGUE,
		ITEMGET,
	};
public:

	GameUI(const D2DResource* D2DResource, const WinSize* winSize, UIRenderMode mode, bool allowChange = true);
	~GameUI();



	void Update();
	void Render();


	void ChangeRenderMode(GameUI::UIRenderMode mode) { InitGameUI(mode); renderMode = mode; }
	void SetRenderModeOnChanging() { renderModeOnChanging = true; }
	void SetGameModeOnChanging(bool change) { gameModeOnChanging = change; }
	void SetCurItemCategory(std::wstring itemCategory) { curItemCategory = itemCategory; }
	void SetDialogueText(std::wstring text, const std::vector<DialogueButtonEvent>& event = std::vector<DialogueButtonEvent>());
	void SetDialogueName(std::wstring name) { dialogueName = name; }

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
	void InitSaveLoadData();
	void InitDialogue();
	void InitItemGet();

	
	// draw text
	IDWriteTextFormat* DynamicTextFormat(const std::wstring& text, const D2D1_RECT_F* textRect, bool* isTransformed = nullptr);
	void DrawTextWithSpacing(IDWriteTextFormat* pTextFormat, const std::wstring& text, const D2D1_RECT_F* rect);
	D2D1_RECT_F GetTextRect(float left, float top, float right, float bottom);
	std::wstring GetFormattedTime(float time = Timer::TotalTime()) const;
	DWRITE_TEXT_METRICS CalculateTextMetrics(const std::wstring& text, float maxWidth);

	// cursor & button
	Cursor startCursor, mainGameCursor_1st, mainGameCursor_2nd, dialogueCursor;
	UINT itemCategoryIdx = 0;
	UINT curSaveSlotPage = 0;
	UINT totalSaveSlotPages = SAVE_SLOT_SIZE / SAVE_SLOTS_PER_PAGE;
	std::wstring curItemCategory;
	float curTime = 0, lastTime = 0;
	std::vector<std::unique_ptr<Button>> startButtons, gameMenuButtons, itemCategoryButtons, saveDataButtons, dialogueButtons;
	std::unordered_map<std::wstring, std::vector<std::unique_ptr<ItemCategoryButton>>> itemButtons;
	std::unordered_map<std::wstring, UINT> itemButtonIdx;
	void UpdateCursorAndButton(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons);
	void UpdateMonsterCursor(Cursor& cursor);
	void UpdateSaveSlotCursor(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons);
	void UpdateUIState();

	// render
	void RenderPlayerStates();
	void RenderMonsterStates();
	void RenderSaveSlot();
	void RenderInGameUI();
	void RenderItemCheck();
	void RenderSaveLoadUI(UIRenderMode renderMode);

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
	// save data
	UITextureRect* saveDataTitleBase = nullptr;
	std::vector<UITextureRect*> saveSlotBases;
	// dialogue
	UITextureRect* dialogueBase = nullptr;
	UITextureRect* nameBase = nullptr;
	std::wstring dialogueText;
	std::wstring dialogueName;
	// icon texture
	TextureRect* playerIcon = nullptr;
	TextureRect* yellowKeyIcon = nullptr;
	TextureRect* blueKeyIcon = nullptr;
	TextureRect* redKeyIcon = nullptr;
	TextureRect* greenKeyIcon = nullptr;
	std::vector<std::unique_ptr<TextureRect>> playerIcons;


	// monster texture&data 
	std::map<UINT, std::tuple<int, std::unique_ptr<TextureRect>, MonsterData>> monsters;

	// Render mode/UI states
	UIState UIstate = UIState::MenuLevel1;
	UIRenderMode renderMode;
	bool renderModeOnChanging = false;
	bool gameModeOnChanging = false;
	bool allowChange = false;

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

