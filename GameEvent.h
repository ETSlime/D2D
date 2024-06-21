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

enum class ItemID : UINT
{
	YELLOW_KEY,
	BLUE_KEY,
	RED_KEY,
	GREEN_KEY,
	GRAY_KEY,
	IRON_KEY,
	SPECIAL_YELLOW_KEY,
	WHITE_KEY,
	YELLOW_KEY_2,
	BLUE_KEY_2,
	RED_KEY_2,
	GREEN_KEY_2,
	BOOK,
	NOTE,
	TELEWARP,
	LUCKY_COIN,
	HOE,
	HOE_2,
	ICE,
	EARTHQUAKE_SCROLL,
	FLYER,
	DOWNSTAIR_FLYER,
	UPSTAIR_FLYER,
	PLACEHOLDER,
	BOOMB,
	LARGE_BOOMB,
	PLACEHOLDER2,
	KINDLE,
	SHOES,
	BAGS,
	PLACEHOLDER3,
	PLACEHOLDER4,
	ATT_GEM,
	DEF_GEM,
	MATK_GEM,
	MDEF_GEM,
	RED_POTION,
	BLUE_POTION,
	GREEN_POTION,
	YELLOW_POTION,
	GREEN_POT,
	PINK_POT,
	BLUE_POT,
	PLACEHOLDER5,
	PLACEHOLDER6,
	PLACEHOLDER7,
	ANTIDOTE,
	UNIVERSAL_ANTIDOTE
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