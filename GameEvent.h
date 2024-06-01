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
	STAIR,
	DEFAULT
};

enum class DoorType
{
	YELLOW,
	BLUE,
	RED,
	SPECIAL
};

enum class StairType
{
	UP,
	DOWN
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

struct ItemEventDescriptor : public EventDescriptor
{
	UINT itemID;
	ItemEventDescriptor() = default;
	ItemEventDescriptor(UINT ID)
	{
		itemID = ID;
	}
};

struct StairEventDescriptor : public EventDescriptor
{
	StairType stairType;;
	Coord newPlayerCoord;
	StairEventDescriptor() = default;
	StairEventDescriptor(StairType type, Coord coord)
	{
		stairType = type;
		newPlayerCoord = coord;
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
	virtual void PostRender() {};

	DirectX::XMFLOAT3* GetPosition();
	void SetupAnimRect(ColliderType type);
	void SetCollision(float LT_x, float LT_y, float RB_x, float RB_y);
	void SetCoord(Coord newCoord);
	void UpdatePositionByCoord(Coord newCoord);
	Coord GetCoord() { return eventCoord; }
	EventType GetEventType() { return eventType; }
	BoundingBox* GetBoundingBox();

	bool destroy = false;

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;

	Coord eventCoord;
	EventType eventType;
	std::wstring eventName;
};