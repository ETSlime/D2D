#pragma once

#include "MagicTowerApp.h"
#include "GameUIGO.h"

class GeneralEventOnTrigger
{
public:

	static MagicTowerApp& mApp;

	static void triggerID001()
	{
		mApp.Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mApp.mD2DResource, &MagicTowerApp::get_instance().curWindowSize, GameUI::MESSAGE));
		dynamic_cast<GameUIGO*>(mApp.mGOs[L"UIDialogueGO"].get())->SetDialogue(L"¤½¤ÎˆöËù¤Ë¤Ïµ½ß_¤Ç¤­¤Þ¤»¤ó¡£");
	}
};

MagicTowerApp& GeneralEventOnTrigger::mApp = MagicTowerApp::get_instance();