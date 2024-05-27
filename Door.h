#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

class Door : public GameEvent
{
public:
	Door(Coord coord, DoorType type, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Door();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }
	void OnPlayerCollision();

private:

	DoorType doorType;
	Map& map = Map::get_instance();

};