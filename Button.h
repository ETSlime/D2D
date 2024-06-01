#pragma once

#include "Util.h"
#include "MagicTowerApp.h"
#include "PlayerGO.h"

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
	static int tutorial()
	{
		MagicTowerApp& mApp = MagicTowerApp::get_instance();

		mApp.mTimer.Stop();
		EnableWindow(mApp.m_hwnd, FALSE);
		MessageBox(mApp.m_hwnd, L"sasa", L"Helper", MB_OK | MB_ICONASTERISK);
		MessageBox(mApp.m_hwnd, L"asfdfsd", L"Helper", MB_OK | MB_ICONASTERISK);
		MessageBox(mApp.m_hwnd, L"sgdsjgo", L"Helper", MB_OK | MB_ICONASTERISK);
		MessageBox(mApp.m_hwnd, L"horfoh", L"Helper", MB_OK | MB_ICONASTERISK);
		EnableWindow(mApp.m_hwnd, TRUE);
		mApp.mTimer.Start();
		
		return 0;
	};

	static int startGame()
	{
		MagicTowerApp& mApp = MagicTowerApp::get_instance();

		mApp.DestroyGO(L"StartMenuGO");
		mApp.Push(L"PlayerGO", std::make_unique<PlayerGO>(Coord(10, 10)));
		mApp.LoadFloor(0);

		return 0;
	}

	static int exitGame()
	{
		PostQuitMessage(0);

		return 0;
	}
};

