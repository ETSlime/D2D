#include "MapStatic.h"
#include "Tile.h"

// define here to prevent link error
std::unordered_map<Coord, UINT> MapStatic::baseFloor[numFloor];
std::vector<std::unique_ptr<EventDescriptor>> MapStatic::eventFloor[numFloor];

std::unordered_set<UINT> MapStatic::walkableTiles =
{
	0, 1, 2, 3, 4, 8, 9, 10, 11, 12
};

UINT MapStatic::eventID = 0;


void MapStatic::GenerateTileMap(UINT floor)
{
	for (UINT i = 0; i < gameWidth; i++)
	{
		for (UINT j = 0; j < gameHeight; j++)
		{
			Map::get_instance().curMap[Coord(i, j)] = new Tile(baseFloor[floor].at(Coord(i, j)), Coord(i, j));
		}
	}
}

template<typename... Args>
std::unique_ptr<EventDescriptor> MapStatic::CreateEventDescriptor(EventType eventType, 
	Coord coord, UINT ID, std::wstring eventName, Args... args)
{
	std::unique_ptr<EventDescriptor> eventDescriptor = nullptr;

	switch (eventType) 
	{
	case EventType::MONSTER:
	{
		auto battleDesc = std::make_unique<BattleEventDescriptor>();
		battleDesc.get()->coord = coord;
		battleDesc.get()->eventID = eventID++;
		battleDesc.get()->eventName = eventName;
		battleDesc.get()->eventType = eventType;
		battleDesc.get()->monsterID = ID;
		battleDesc.get()->monsterName = L"Slime";
		eventDescriptor = std::move(battleDesc);
		break;
	}

	case EventType::DOOR:
	{
		std::unique_ptr<DoorEventDescriptor> doorDesc = std::make_unique<DoorEventDescriptor>();
		doorDesc.get()->coord = coord;
		doorDesc.get()->eventID = eventID++;
		doorDesc.get()->eventName = eventName;
		doorDesc.get()->eventType = eventType;
		doorDesc.get()->doorType = static_cast<DoorType>(ID);
		eventDescriptor = std::move(doorDesc);
		break;
	}
	case EventType::ITEM:
	{
		std::unique_ptr<ItemEventDescriptor> itemDesc = std::make_unique<ItemEventDescriptor>();
		itemDesc.get()->coord = coord;
		itemDesc.get()->eventID = eventID++;
		itemDesc.get()->eventName = eventName;
		itemDesc.get()->eventType = eventType;
		itemDesc.get()->itemID = ID;
		eventDescriptor = std::move(itemDesc);
		break;
	}
	case EventType::STAIR:
	{
		Coord newPlayerCoord;
		(void)std::initializer_list<int>{(newPlayerCoord = args, 0)...};
		std::unique_ptr<StairEventDescriptor> stairDesc = std::make_unique<StairEventDescriptor>();
		stairDesc.get()->coord = coord;
		stairDesc.get()->eventID = eventID++;
		stairDesc.get()->eventName = eventName;
		stairDesc.get()->eventType = eventType;
		stairDesc.get()->stairType = static_cast<StairType>(ID);
		stairDesc.get()->newPlayerCoord = newPlayerCoord;
		eventDescriptor = std::move(stairDesc);
		break;
	}
	default:
		return std::unique_ptr<EventDescriptor>(nullptr);
	}

	return eventDescriptor;
}

void MapStatic::BuildFloor0()
{
	UINT floorNum = 0;
	for (UINT i = 0; i < gameWidth; i++)
	{
		for (UINT j = 0; j < gameHeight; j++)
		{
			baseFloor[floorNum].insert({ Coord(i, j), 11 });
		}
	}

	GenerateTileMap(floorNum);

	// event
	eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(4, 5), 0, L"Monster001"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(4, 6), 0, L"Monster002"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(4, 7), 0, L"Monster003"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(5, 5), 0, L"Monster004"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(5, 6), 0, L"Monster005"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(5, 7), 0, L"Monster006"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(5, 8), 0, L"Monster007"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(3, 5), 0, L"Monster008"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(3, 6), 0, L"Monster009"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(3, 7), 0, L"Monster010"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(3, 8), 0, L"Monster011"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(2, 5), 0, L"Monster012"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::DOOR, Coord(3, 4), static_cast<UINT>(DoorType::YELLOW), L"Door001"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::DOOR, Coord(1, 4), static_cast<UINT>(DoorType::BLUE), L"Door003"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::DOOR, Coord(2, 4), static_cast<UINT>(DoorType::YELLOW), L"Door002"));
	//eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::ITEM, Coord(6, 4), 0, L"Item001"));
	eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::STAIR, Coord(6, 5), static_cast<UINT>(StairType::UP), L"Stair001", Coord(1, 1)));
	
}

void MapStatic::BuildFloor1()
{
	UINT floorNum = 1;
	for (UINT i = 0; i < gameWidth; i++)
	{
		for (UINT j = 0; j < gameHeight; j++)
		{
			baseFloor[floorNum].insert({ Coord(i, j), 12 });
		}
	}
	GenerateTileMap(floorNum);

	eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::MONSTER, Coord(4, 6), 0, L"Monster101"));
	eventFloor[floorNum].push_back(CreateEventDescriptor(EventType::STAIR, Coord(6, 5), static_cast<UINT>(StairType::DOWN), L"Stair001", Coord(2,2)));
}