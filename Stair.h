#pragma once

#include "GameEvent.h"
#include "Player.h"

class AnimationRect;
class ChangeMapEffect;

class Stair : public GameEvent
{
public:
	Stair(Coord coord, StairType type, Coord newPlayerCoord, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Stair();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	AnimationRect* GetanimRect() { return animRect; }
	void OnPlayerCollision(Coroutine& coro, StairType type);
	void SetCurrentFloor(int number) { floorNumber = number; }

private:
	
	StairType stairType;
	int floorNumber = 0;
	Coord newPlayerCoord;
	MagicTowerApp& mApp = MagicTowerApp::get_instance();
	
};