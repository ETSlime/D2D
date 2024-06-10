#include "ButtonOnClick.h"
#include "ChangeMapEffect.h"

std::unique_ptr<Coroutine> ButtonOnClick::coro = std::make_unique<Coroutine>();

void ButtonOnClick::fadeCallback(Coroutine& coro)
{
	MagicTowerApp& mApp = MagicTowerApp::get_instance();

	if (coro.getState() == 0)
	{
		mApp.Push(L"PlayerGO", std::make_unique<PlayerGO>(Coord(0, 0)));
		Player::player->allowControl = false;
		Player::player->PlayFadeEffect(true);
		coro.yield(Player::player->fadeEffect->GetFadeSpeed());
	}
	if (coro.getState() == 1)
	{
		mApp.DestroyGO(L"StartMenuGO");
		Player::player->renderPlayer = true;
		mApp.LoadFloor(0);
		Player::player->PlayFadeEffect(false);
		Player::player->allowControl = true;
		coro.setComplete();
	}
}

int ButtonOnClick::tutorial()
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

int ButtonOnClick::startGame()
{
	coro.get()->setCallback([](Coroutine& coro)
		{ fadeCallback(coro); });
	(*coro.get())();


	return 0;
}

int ButtonOnClick::exitGame()
{
	PostQuitMessage(0);

	return 0;
}