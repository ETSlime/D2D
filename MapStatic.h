#pragma once

#include "Util.h"
#include "Map.h"
#include "GameEvent.h"



struct EventParams {
	// Virtual destructor to ensure the base class is polymorphic
	virtual ~EventParams() = default;
	EventType type;
	Coord coord;

	EventParams(EventType type, Coord coord) : type(type), coord(coord) {}
};


struct MonsterParams : public EventParams {
	UINT monsterID;

	MonsterParams(EventType type, Coord coord, UINT monsterID)
		: EventParams(type, coord), monsterID(monsterID) {}
};


struct ItemParams : public EventParams {
	ItemID itemID;

	ItemParams(EventType type, Coord coord, ItemID itemID)
		: EventParams(type, coord), itemID(itemID) {}

	~ItemParams() { std::cout << "item destroyed"; }
};


struct DoorParams : public EventParams {
	DoorType doorType;

	DoorParams(EventType type, Coord coord, DoorType doorType)
		: EventParams(type, coord), doorType(doorType) {}
};


struct StairParams : public EventParams {
	StairType stairType;
	Coord newPlayerCoord;

	StairParams(EventType type, Coord coord, StairType stairType, Coord newPlayerCoord)
		: EventParams(type, coord), stairType(stairType), newPlayerCoord(newPlayerCoord) {}
};

struct NPCParams : public EventParams {
	UINT NPCID;
	UINT dialogueID;
	NPCParams(EventType type, Coord coord, UINT NPCID, UINT dialogueID)
		: EventParams(type, coord), NPCID(NPCID), dialogueID(dialogueID) {}
};

class MapStatic
{
public:
	// Coord: map coordinate
	// UINT: tile Index

	static const UINT gameWidth = 13;
	static const UINT gameHeight = 13;
	static const UINT numFloor = 100;

	// static map data
	static std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>> eventParams;
	static std::unordered_map<int, std::unordered_map<Coord, UINT>> mapTileIdx;
	static std::vector<std::unique_ptr<EventDescriptor>> eventFloor[numFloor];
	static std::unordered_set<UINT> walkableTiles;

	static UINT eventID;

	static void BuildFloor(int floorNum);

	template<typename... Args>
	static std::unique_ptr<EventDescriptor> CreateEventDescriptor(EventType eventType, 
		Coord coord, UINT eventID, std::wstring eventName = L"DefaultEventName", Args... args);
	static void GenerateTileMap(UINT floor);

};

