#pragma once

#include "Util.h"
#include "MagicTowerApp.h"
#include "PlayerGO.h"
#include "Player.h"
#include "Database.h"

typedef std::function<void()> OnClick;

class ButtonOnClick
{

public:

	static std::unique_ptr<Coroutine> coro;

	static MagicTowerApp& mApp;

	static void startGameFadeCallback(Coroutine& coro);

	static void returnTitleFadeCallback(Coroutine& coro);

	static void symmetricFlyerCallback(Coroutine& coro);

	static int tutorial();

	static int startGame();

	static int startNewGame();

	static int exitGame();

	// main game UI

	static int itemCheck();

	static int saveGameUI();

	static int loadGameUI();

	static int title();

	// save/load

	static int saveData(UINT slotNum);

	static int loadData(UINT slotNum);

	static int showItemList(std::wstring itemCategory);

	// item buttons
	
	static void symmetricFlyer();
};

struct Button
{
	Button(std::wstring text, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, const WinSize* curWinSize, OnClick pOnClick, bool Selected = false) :
		text(text), position(position), textWidth(size.x), textHeight(size.y), pOnClick(pOnClick), isSelected(Selected)
	{
		UpdateTextRect(curWinSize);
	};

	template<typename Callable, typename... Args>
	void overrideOnClick(Callable&& func, Args&&... args)
	{
		pOnClick = std::bind(std::forward<Callable>(func), std::forward<Args>(args)...);
	}

	virtual ~Button() = default;

	DirectX::XMFLOAT3 position;
	float textWidth;
	float textHeight;
	std::wstring text;
	bool isSelected = false;
	D2D1_RECT_F textRect;


	OnClick pOnClick;


	void UpdateTextRect(const WinSize* curWinSize)
	{
		textRect.left = curWinSize->width * position.x;
		textRect.top = curWinSize->height * position.y;
		textRect.right = textRect.left + textWidth;
		textRect.bottom = textRect.top + textHeight;
	}

};

struct DialogueButton : Button
{
	DialogueButton(std::wstring text, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, const WinSize* curWinSize, OnClick pOnClick, DialogueButtonEvent event, bool Selected = false) :
		Button(text, position, size, curWinSize, pOnClick, Selected), dialogueButtonEvent(event)
	{
		switch (dialogueButtonEvent)
		{
		case DialogueButtonEvent::ADDHP:
			this->pOnClick = ButtonOnClick::tutorial;
			break;
		default:
			break;
		}
	}

	DialogueButtonEvent dialogueButtonEvent;
};

struct ItemCategoryButton : Button
{
	ItemCategoryButton(std::wstring text, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, const WinSize* curWinSize, OnClick pOnClick, TextureRect* icon, bool Selected = false) :
		Button(text, position, size, curWinSize, pOnClick, Selected), itemIcon(icon) {}

	void UpdateCount(UINT count)
	{
		itemCount = count;
		if (itemCount <= 1)
			return;
		text = text + L"   X " + std::to_wstring(itemCount);
	}

	UINT itemCount = 0;

	std::unique_ptr<TextureRect> itemIcon;
};
