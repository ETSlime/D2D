#pragma once

#include "Util.h"
#include "MagicTowerApp.h"
#include "PlayerGO.h"
#include "Player.h"

typedef int (*OnClick)();

struct Button
{
	Button(std::wstring text, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, const WinSize* curWinSize, OnClick pOnClick, bool Selected = false) :
		text(text), position(position), textWidth(size.x), textHeight(size.y), pOnClick(pOnClick), isSelected(Selected)
	{
		UpdateTextRect(curWinSize);
	};
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

class ButtonOnClick
{

public:

	static std::unique_ptr<Coroutine> coro;

	static MagicTowerApp& mApp;

	static void startGameFadeCallback(Coroutine& coro);

	static void returnTitleFadeCallback(Coroutine& coro);

	static int tutorial();

	static int startGame();

	static int exitGame();

	static int save();

	static int load();

	static int itemCheck();

	static int title();
};
