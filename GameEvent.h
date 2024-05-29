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
	DOOR,
	TERRAIN,
	DEFAULT
};

enum class DoorType
{
	YELLOW,
	BLUE,
	RED,
	SPECIAL
};

struct EventDescriptor 
{
	UINT eventID;
	Coord coord;
	std::wstring eventName;
	EventType eventType;

	virtual ~EventDescriptor() = default;
};

struct BattleEventDescriptor : public EventDescriptor 
{
	UINT monsterID;
	std::wstring monsterName;
	BattleEventDescriptor() = default;
	BattleEventDescriptor(UINT ID, std::wstring name) 
	{
		monsterID = ID;
		monsterName = name;
		eventType = EventType::MONSTER;
	}
};


struct DoorEventDescriptor : public EventDescriptor
{
	DoorType doorType;
	DoorEventDescriptor() = default;
	DoorEventDescriptor(DoorType type)
	{
		doorType = type;
		eventType = EventType::DOOR;
	}
};

class GameEvent
{
public:
	GameEvent(Coord coord, DirectX::XMFLOAT3 size, EventType type = EventType::DEFAULT, 
		std::wstring eventName = L"DefaultEventName");
	virtual ~GameEvent();

	virtual void Update() = 0;
	virtual void Render() = 0;
	DirectX::XMFLOAT3* GetPosition();
	void SetCollision(float LT_x, float LT_y, float RB_x, float RB_y);
	void SetCoord(Coord newCoord);
	void UpdatePositionByCoord(Coord newCoord);
	Coord GetCoord() { return eventCoord; }

	BoundingBox* GetBoundingBox();

	bool destroy = false;

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;

	Coord eventCoord;
	EventType eventType;
	std::wstring eventName;
};