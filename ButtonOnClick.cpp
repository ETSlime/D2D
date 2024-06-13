#include "ButtonOnClick.h"
#include "ChangeMapEffect.h"
#include "GameUIGO.h"

std::unique_ptr<Coroutine> ButtonOnClick::coro;
MagicTowerApp& ButtonOnClick::mApp = MagicTowerApp::get_instance();

void ButtonOnClick::startGameFadeCallback(Coroutine& coro)
{
	// start fade in
	if (coro.getState() == 0)
	{
		// changing game mode
		dynamic_cast<GameUIGO*>(mApp.startMenuGO.get())->SetChangeGameMode(true);
		// create player GO
		mApp.Push(L"PlayerGO", std::make_unique<PlayerGO>(Coord(0, 0)));
		Player::player->allowControl = false;
		Player::player->PlayFadeEffect(true);
		// fade in effect
		coro.yield(Player::player->fadeEffect->GetFadeSpeed());
	}
	// start fadeout, enter gameplay mode
	if (coro.getState() == 1)
	{
		mApp.SetGameMode(GameMode::GAMEPLAY);
		//mApp.DestroyGO(L"StartMenuGO");
		Player::player->renderPlayer = true;
		mApp.LoadFloor(0);
		Player::player->PlayFadeEffect(false);
		Player::player->allowControl = true;
		// finish changing game mode
		dynamic_cast<GameUIGO*>(mApp.startMenuGO.get())->SetChangeGameMode(false);
		coro.setComplete();
	}
}

void ButtonOnClick::returnTitleFadeCallback(Coroutine& coro)
{
	// start fade in
	if (coro.getState() == 0)
	{
		Player::player->allowControl = false;
		Player::player->PlayFadeEffect(true);
		coro.yield(Player::player->fadeEffect->GetFadeSpeed());
	}
	// start fade out, return to title
	if (coro.getState() == 1)
	{
		mApp.SetGameMode(GameMode::TITLE);
		Player::player->PlayFadeEffect(false);
		mApp.ReturnTitle();
		coro.setComplete();
	}

}

int ButtonOnClick::tutorial()
{
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

int ButtonOnClick::startGame()
{
	coro = std::make_unique<Coroutine>();
	coro.get()->setCallback([](Coroutine& coro)
		{ startGameFadeCallback(coro); });
	(*coro.get())();

	return 0;
}

int ButtonOnClick::exitGame()
{
	PostQuitMessage(0);

	return 0;
}

int ButtonOnClick::save()
{
	return 0;
}

int ButtonOnClick::load()
{
	return 0;
}

int ButtonOnClick::itemCheck()
{
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->ChangeUIMode(GameUI::UIRenderMode::ITEMCHECK);
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetChangeUIMode();
	return 0;
}

int ButtonOnClick::title()
{
	mApp.SetGameMode(GameMode::GAMEPLAY);
	coro = std::make_unique<Coroutine>();
	coro.get()->setCallback([](Coroutine& coro)
		{ returnTitleFadeCallback(coro); });
	(*coro.get())();

	return 0;
}