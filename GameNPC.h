#pragma once

#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"
#include "Player.h"
#include "Database.h"
#include "GameUI.h"

class GameNPC : public GameEvent
{
public:
	GameNPC(Coord coord, UINT GameNPCID, UINT dialogueID, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~GameNPC();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

	void OnPlayerCollision(Coroutine& coro);

	UINT GameNPCID;
	UINT dialogueID;

private:

	MagicTowerApp& mApp = MagicTowerApp::get_instance();
};