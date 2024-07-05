#include "ButtonOnClick.h"
#include "ChangeMapEffect.h"
#include "GameUIGO.h"
#include "SaveData.h"
#include "Tile.h"

std::unique_ptr<Coroutine> ButtonOnClick::coro;
MagicTowerApp& ButtonOnClick::mApp = MagicTowerApp::get_instance();

void ButtonOnClick::startGameFadeCallback(Coroutine& coro)
{
	// start fade in
	if (coro.getState() == 0)
	{
		// changing game mode
		if (mApp.startMenuGO) dynamic_cast<GameUIGO*>(mApp.startMenuGO.get())->SetChangeGameMode(true);
		if (mApp.gameUI) dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetChangeGameMode(true);
		// create player GO
		mApp.Push(L"PlayerGO", std::make_unique<PlayerGO>(Coord(0, 0)));
		Player::player->SetAllowControl(false);
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
		mApp.LoadFloor(Player::player->GetCurFloor());
		Player::player->PlayFadeEffect(false);
		Player::player->SetAllowControl(true);
		// finish changing game mode
		dynamic_cast<GameUIGO*>(mApp.startMenuGO.get())->SetChangeGameMode(false);
		dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetChangeGameMode(false);
		coro.setComplete();
	}
}

void ButtonOnClick::returnTitleFadeCallback(Coroutine& coro)
{
	// start fade in
	if (coro.getState() == 0)
	{
		Player::player->SetAllowControl(false);
		Player::player->PlayFadeEffect(true);
		coro.yield(Player::player->fadeEffect->GetFadeSpeed());
	}
	// start fade out, return to title
	if (coro.getState() == 1)
	{
		mApp.SetGameMode(GameMode::TITLE);
		Player::player->PlayFadeEffect(false);
		mApp.ReturnTitle();
		mApp.startMenuGO.get()->SetIsValid(true);
		coro.setComplete();
	}

}

void ButtonOnClick::symmetricFlyerCallback(Coroutine& coro)
{
	// start fade in
	if (coro.getState() == 0)
	{
		Player::player->UseItem(ItemID::SYMMETRIC_FLYER);
		Player::player->SetAllowControl(false);
		Player::player->animRect->StartFadeOut();
		coro.yield(2);
	}
	if (coro.getState() == 1)
	{
		Player::player->UpdatePositionByCoord(Player::player->GetCoord());
		Player::player->animRect->StartFadeIn();
		coro.yield(2);
	}
	if (coro.getState() == 2)
	{
		Player::player->SetAllowControl(true);
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

int ButtonOnClick::startNewGame()
{
	MapStatic::InitMap();
	Timer::ClearTotalTime();
	startGame();

	return 0;
}

int ButtonOnClick::exitGame()
{
	PostQuitMessage(0);

	return 0;
}

int ButtonOnClick::itemCheck()
{
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->ChangeUIMode(GameUI::UIRenderMode::ITEMCHECK);
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetChangeUIMode();
	return 0;
}

int ButtonOnClick::saveGameUI()
{
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->ChangeUIMode(GameUI::UIRenderMode::SAVEDATA);
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetChangeUIMode();
	return 0;
}

int ButtonOnClick::loadGameUI()
{
	if (mApp.startMenuGO)
		mApp.startMenuGO.get()->SetIsValid(false);
	if (mApp.gameUI)
		mApp.gameUI.get()->SetIsValid(true);
	else
		mApp.gameUI = std::make_unique<GameUIGO>(&mApp.mD2DResource, &mApp.curWindowSize, GameUI::LOADDATA);
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->ChangeUIMode(GameUI::UIRenderMode::LOADDATA);
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

int ButtonOnClick::showItemList(std::wstring itemCategory)
{
	dynamic_cast<GameUIGO*>(mApp.gameUI.get())->SetItemCategory(itemCategory);
	return 0;
}

int ButtonOnClick::saveData(UINT slotNum)
{
	char buffer[16]; // Enough to hold "SaveData" + 3 digits + ".dat" + null terminator
	std::snprintf(buffer, sizeof(buffer), "SaveData%03d.dat", slotNum);
	std::string filename(buffer);
	SaveData::SaveGame(filename);

	return 0;
}

int ButtonOnClick::loadData(UINT slotNum)
{
	char buffer[16]; // Enough to hold "SaveData" + 3 digits + ".dat" + null terminator
	std::snprintf(buffer, sizeof(buffer), "SaveData%03d.dat", slotNum);
	std::string fileName(buffer);
	GameState gameState;
	if (SaveData::LoadGame(fileName, gameState) == -1)
		return 0;

	mApp.ReturnTitle();
	startGame();

	Player::player->playerData = gameState.playerData;
	Player::player->SetCoord(gameState.playerCoord);
	Player::player->UpdatePositionByCoord(gameState.playerCoord);
	Player::player->walkingSteps = gameState.walkingSteps;
	Player::player->curFloor = gameState.curFloor;
	Player::player->items = gameState.inventoryItems;

	MapStatic::mapTileIdx.clear();
	MapStatic::mapTileIdx = gameState.mapTileIdx;
	MapStatic::eventParams.clear();
	MapStatic::eventParams = std::move(gameState.eventParams);

	Timer::SetTotalTime(gameState.totalTime);
	return 0;
}

void ButtonOnClick::symmetricFlyer()
{
	mApp.SetShakeEffect(true);
	mApp.SetGameMode(GameMode::GAMEPLAY);

	//Coord oldCoord = Player::player->GetCoord();
	//Coord newCoord = Coord(MapStatic::gameWidth - oldCoord.x, MapStatic::gameHeight - oldCoord.y);

	//// check target position tile walkable
	//if (!Map::get_instance().curMap[newCoord].get()->GetIsWalkable())
	//{
	//	mApp.Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mApp.mD2DResource, &mApp.curWindowSize, GameUI::ITEMGET, false));
	//	dynamic_cast<GameUIGO*>(mApp.mGOs[L"UIDialogueGO"].get())->SetDialogue(L"その場所には到達できません。");
	//	return;
	//}

	//// check if events on target position
	//for (auto& curEvent : Map::get_instance().curEvents)
	//{
	//	if (curEvent.second.get()->GetCoord() == newCoord)
	//	{
	//		mApp.Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mApp.mD2DResource, &mApp.curWindowSize, GameUI::ITEMGET, false));
	//		dynamic_cast<GameUIGO*>(mApp.mGOs[L"UIDialogueGO"].get())->SetDialogue(L"その場所には到達できません。");
	//		return;
	//	}
	//}
	//Player::player->SetCoord(newCoord);
	//coro = std::make_unique<Coroutine>();
	//coro.get()->setCallback([](Coroutine& coro)
	//	{ symmetricFlyerCallback(coro); });
	//(*coro.get())();
}