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
	ARROW,
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

enum class ArrowDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class TerrainType
{
	HP,
	BLOCK
};

struct EventDescriptor 
{
	UINT eventID = 0;
	Coord coord;
	std::wstring eventName;
	EventType eventType = EventType::DEFAULT;

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
	ItemID itemID;
	ItemEventDescriptor() = default;
	ItemEventDescriptor(ItemID ID)
	{
		itemID = ID;
	}
};

struct StairEventDescriptor : public EventDescriptor
{
	StairType stairType;
	Coord newPlayerCoord;
	StairEventDescriptor() = default;
	StairEventDescriptor(StairType type, Coord coord)
	{
		stairType = type;
		newPlayerCoord = coord;
	}
};

struct NPCEventDescriptor : public EventDescriptor
{
	UINT NPCID;
	UINT dialogueID;
	NPCEventDescriptor() = default;
	NPCEventDescriptor(UINT npc, UINT dialogue)
	{
		NPCID = npc;
		dialogueID = dialogue;
	}
};

struct ArrowEventDescriptor : public EventDescriptor
{
	ArrowDirection arrowDir;
	ArrowEventDescriptor() = default;
	ArrowEventDescriptor(ArrowDirection dir)
	{
		arrowDir = dir;
	}
};

struct TerrainEventDescriptor : public EventDescriptor
{
	TerrainType terrainType;
	TerrainEventDescriptor() = default;
	TerrainEventDescriptor(TerrainType type)
	{
		terrainType = type;
	}
};

struct GeneralEventDescriptor : public EventDescriptor
{
	UINT triggerID;
	bool triggerOnce;
	ColliderType colliderType;

	GeneralEventDescriptor() = default;
	GeneralEventDescriptor(UINT ID, bool once, ColliderType type)
	{
		triggerID = ID;
		triggerOnce = once;
		colliderType = type;
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
	void SetCoord(Coord newCoord);
	void UpdatePositionByCoord(Coord newCoord);
	Coord GetCoord() { return eventCoord; }
	EventType GetEventType() { return eventType; }
	BoundingBox* GetBoundingBox();
	std::wstring GetEventName() { return eventName; }
	bool GetDestroyed() { return destroy; }
	

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
	bool destroy = false;

	Coord eventCoord;
	EventType eventType;
	std::wstring eventName;
};