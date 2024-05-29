#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"
#include "Player.h"

class Monster : public GameEvent
{
public:
	Monster(Coord coord, UINT monsterID, std::wstring eventName = L"DefaultEventName", DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Monster();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

	void OnPlayerCollision(Coroutine& coro);

private:
	
	

};