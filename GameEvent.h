#pragma once
#pragma once

#include "Animator.h"
#include "Map.h"

class AnimationRect;

enum class EventType
{
	PLAYER,
	MONSTER,
	NPC,
	ITEM,
	TERRAIN,
	DEFAULT
};

class GameEvent
{
public:
	GameEvent(Coord coord, DirectX::XMFLOAT3 size, EventType type = EventType::DEFAULT);
	virtual ~GameEvent();

	virtual void Update() = 0;
	virtual void Render() = 0;
	DirectX::XMFLOAT3* GetPosition();
	void SetCollision(float LT_x, float LT_y, float RB_x, float RB_y);
	void SetCoord(Coord newCoord);
	void UpdatePositionByCoord(Coord newCoord);
	Coord GetCoord() { return eventCoord; }

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;

	Coord eventCoord;
	EventType eventType;
};