#pragma once

#include "MagicTowerApp.h"
#include "GameUIGO.h"
#include "Player.h"

class GeneralEventOnTrigger
{
public:

	static MagicTowerApp& mApp;

	static void triggerID001()
	{
		mApp.Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mApp.mD2DResource, &MagicTowerApp::get_instance().curWindowSize, GameUI::MESSAGE));
		dynamic_cast<GameUIGO*>(mApp.mGOs[L"UIDialogueGO"].get())->SetDialogue(L"¤½¤ÎˆöËù¤Ë¤Ïµ½ß_¤Ç¤­¤Þ¤»¤ó¡£");
	}

	static void RedPotion()
	{
		Player::player->ChangeHP(50);
	}
	static void BluePotion()
	{
		Player::player->ChangeHP(50);
	}
	static void GreenPotion()
	{
		Player::player->ChangeHP(50);
	}
	static void YellowPotion()
	{
		Player::player->ChangeHP(50);
	}
	static void AtkGem()
	{
		Player::player->ChangeAtk(50);
	}
	static void DefGem()
	{
		Player::player->ChangeDef(50);
	}
};

MagicTowerApp& GeneralEventOnTrigger::mApp = MagicTowerApp::get_instance();