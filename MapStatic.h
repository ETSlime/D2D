#pragma once

#include "Util.h"
#include "IGameObj.h"
#include "Map.h"
#include "GameEvent.h"

class MapStatic
{
public:
	// Coord: map coordinate
	// UINT: tile Index

	static const UINT gameWidth = 13;
	static const UINT gameHeight = 13;
	static const UINT numFloor = 100;

	static std::unordered_map<Coord, UINT> baseFloor[numFloor];
	static std::vector<std::unique_ptr<EventDescriptor>> eventFloor[numFloor];
	static std::unordered_set<UINT> walkableTiles;

	static UINT eventID;

	static void BuildFloor0();
	static void BuildFloor1();

	static std::unique_ptr<EventDescriptor> CreateEventDescriptor(EventType eventType, 
		Coord coord, UINT eventID, std::wstring eventName = L"DefaultEventName");
	static void GenerateTileMap(UINT floor);

};