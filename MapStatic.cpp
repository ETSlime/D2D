#include "MapStatic.h"
#include "Tile.h"
#include "Database.h"

// define here to prevent link error
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
			Map::get_instance().curMap[Coord(i, j)] = std::make_shared<Tile>(mapTileIdx[floor].at(Coord(i, j)), Coord(i, j));
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
		std::unique_ptr<BattleEventDescriptor> battleDesc = std::make_unique<BattleEventDescriptor>();
		battleDesc.get()->coord = coord;
		battleDesc.get()->eventID = eventID++;
		battleDesc.get()->eventName = eventName;
		battleDesc.get()->eventType = eventType;
		battleDesc.get()->monsterID = ID;
		battleDesc.get()->monsterName = Database::monsterDatabase[ID].name;
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
		itemDesc.get()->itemID = static_cast<ItemID>(ID);;
		eventDescriptor = std::move(itemDesc);
		break;
	}
	case EventType::STAIR:
	{
		Coord newPlayerCoord;
		(void)std::initializer_list<int>{
			([&](auto&& arg)
			{
				if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, Coord>)
				{
					newPlayerCoord = arg;
				}
				return 0;
			}(args), 0)...
		};
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
	case EventType::NPC:
	{
		UINT dialogueID = 0;
		(void)std::initializer_list<int>{
			([&](auto&& arg)
				{
					if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, UINT>)
					{
						dialogueID = arg;
					}
					return 0;
				}(args), 0)...
		};
		std::unique_ptr<NPCEventDescriptor> NPCDesc = std::make_unique<NPCEventDescriptor>();
		NPCDesc.get()->coord = coord;
		NPCDesc.get()->eventID = eventID++;
		NPCDesc.get()->eventName = eventName;
		NPCDesc.get()->eventType = eventType;
		NPCDesc.get()->NPCID = ID;
		NPCDesc.get()->dialogueID = dialogueID;
		eventDescriptor = std::move(NPCDesc);
		break;
	}
	case EventType::ARROW:
	{
		std::unique_ptr<ArrowEventDescriptor> arrowDesc = std::make_unique<ArrowEventDescriptor>();
		arrowDesc.get()->coord = coord;
		arrowDesc.get()->eventID = eventID++;
		arrowDesc.get()->eventName = eventName;
		arrowDesc.get()->eventType = eventType;
		arrowDesc.get()->arrowDir = static_cast<ArrowDirection>(ID);
		eventDescriptor = std::move(arrowDesc);
		break;
	}
	case EventType::TERRAIN:
	{
		bool isBlocking = false;
		(void)std::initializer_list<int>{
			([&](auto&& arg)
				{
					if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, bool>)
					{
						isBlocking = arg;
					}
					return 0;
				}(args), 0)...
		};
		std::unique_ptr<TerrainEventDescriptor> terrainDesc = std::make_unique<TerrainEventDescriptor>();
		terrainDesc.get()->coord = coord;
		terrainDesc.get()->eventID = eventID++;
		terrainDesc.get()->eventName = eventName;
		terrainDesc.get()->eventType = eventType;
		terrainDesc.get()->terrainType = static_cast<TerrainType>(ID);
		terrainDesc.get()->blocking = isBlocking;
		eventDescriptor = std::move(terrainDesc);
		break;
	}
	case EventType::DEFAULT:
	{
		bool triggerOnce = false;
		ColliderType colliderType = ColliderType::BLOCKING;
		(void)std::initializer_list<int>{
			([&](auto&& arg)
				{
					if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, bool>)
					{
						triggerOnce = arg;
					}
					else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, ColliderType>)
					{
						colliderType = arg;
					}
					return 0;
				}(args), 0)...
		};
		std::unique_ptr<GeneralEventDescriptor> eventDesc = std::make_unique<GeneralEventDescriptor>();
		eventDesc.get()->coord = coord;
		eventDesc.get()->eventID = eventID++;
		eventDesc.get()->eventName = eventName;
		eventDesc.get()->eventType = eventType;
		eventDesc.get()->triggerID = ID;
		eventDesc.get()->triggerOnce = triggerOnce;
		eventDesc.get()->colliderType = colliderType;
		eventDescriptor = std::move(eventDesc);
		break;
	}
	default:
		return std::unique_ptr<EventDescriptor>(nullptr);
	}

	return eventDescriptor;
}

void MapStatic::BuildFloor(int floorNum)
{
	GenerateTileMap(floorNum);

	// event
	for (const auto& [name, params] : MapStatic::eventParams[floorNum]) {
		if (auto stairParams = dynamic_cast<StairParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(stairParams->type, stairParams->coord, static_cast<UINT>(stairParams->stairType), name, stairParams->newPlayerCoord));
		}
		else if (auto monsterParams = dynamic_cast<MonsterParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(monsterParams->type, monsterParams->coord, monsterParams->monsterID, name));
		}
		else if (auto itemParams = dynamic_cast<ItemParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(itemParams->type, itemParams->coord, static_cast<UINT>(itemParams->itemID), name));
		}
		else if (auto doorParams = dynamic_cast<DoorParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(doorParams->type, doorParams->coord, static_cast<UINT>(doorParams->doorType), name));
		}
		else if (auto npcParams = dynamic_cast<NPCParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(npcParams->type, npcParams->coord, npcParams->NPCID, name, npcParams->dialogueID));
		}
		else if (auto arrowParams = dynamic_cast<ArrowParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(arrowParams->type, arrowParams->coord, static_cast<UINT>(arrowParams->arrowDir), name));
		}
		else if (auto terrainParams = dynamic_cast<TerrainParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(terrainParams->type, terrainParams->coord, static_cast<UINT>(terrainParams->terrainType), name, terrainParams->blocking));
		}
		else if (auto generalEventParams = dynamic_cast<GeneralEventParams*>(params.get())) {
			eventFloor[floorNum].push_back(CreateEventDescriptor(generalEventParams->type, generalEventParams->coord, generalEventParams->triggerID, name, generalEventParams->triggerOnce, generalEventParams->colliderType));
		}
	}
	
}

void MapStatic::InitMap()
{
	/*
Explanation:
The static member variable 'eventParams' is initialized using a lambda function to ensure all elements are inserted using the 'emplace' method.
This avoids copying 'std::unique_ptr', which is non-copyable. 'std::make_unique' is used to create 'unique_ptr' instances which are then moved
into the map. This approach ensures that the initialization is done correctly and avoids potential issues with non-copyable objects.
*/
	eventParams = []
		{
			std::unordered_map<int, std::map<std::wstring, std::unique_ptr<EventParams>>> tempEventParams;

			// Using emplace to avoid copying std::unique_ptr, which is non-copyable.
			std::vector<std::tuple<UINT, std::unique_ptr<EventParams>>> eventParams;
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 1 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 2 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 6 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 7 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 8 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 5, 9 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 6, 9 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 1 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 2 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 6 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 7 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 8 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 7, 9 }, DoorType::FENCE)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<NPCParams>(EventType::NPC, Coord{ 6, 6}, NPCType::ELF, DialogueID_001_1F_ELF)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<DoorParams>(EventType::DOOR, Coord{ 6, 7}, DoorType::RED)));
			eventParams.push_back(std::make_tuple(Floor::FLOOR_0, std::make_unique<StairParams>(EventType::STAIR, Coord{ 6, 8 }, StairType::UP, Coord{ 2, 2 })));

			eventParams.push_back(std::make_tuple(Floor::FLOOR_1, std::make_unique<StairParams>(EventType::STAIR, Coord{ 6, 5 }, StairType::DOWN, Coord{ 2, 2 })));

			for (int i = 0; i < eventParams.size(); i++)
			{
				std::wstring eventName = std::get<1>(eventParams[i]).get()->GetEventName() + std::to_wstring(i);

				tempEventParams[std::get<0>(eventParams[i])].emplace(eventName, std::move(std::get<1>(eventParams[i])));
			}

			tempEventParams[0].emplace(L"NPC001", std::make_unique<NPCParams>(EventType::NPC, Coord{ 12, 1 }, 0, 2));
			//tempEventParams[0].emplace(L"Monster001", std::make_unique<MonsterParams>(EventType::MONSTER, Coord{ 12, 5 }, 0));
			tempEventParams[0].emplace(L"Item001", std::make_unique<ItemParams>(EventType::ITEM, Coord{ 3, 6 }, ItemID::SYMMETRIC_FLYER));
			tempEventParams[0].emplace(L"Item002", std::make_unique<ItemParams>(EventType::ITEM, Coord{ 3, 7 }, ItemID::ATK_GEM));
			tempEventParams[0].emplace(L"Item003", std::make_unique<ItemParams>(EventType::ITEM, Coord{ 3, 8 }, ItemID::DEF_GEM));
			//tempEventParams[0].emplace(L"Door001", std::make_unique<DoorParams>(EventType::DOOR, Coord{ 4, 6 }, DoorType::YELLOW));
			tempEventParams[0].emplace(L"Item004", std::make_unique<ItemParams>(EventType::ITEM, Coord{ 4, 10 }, ItemID::BOOK));
			//tempEventParams[0].emplace(L"Door002", std::make_unique<DoorParams>(EventType::DOOR, Coord{ 4, 7 }, DoorType::BLUE));
			//tempEventParams[0].emplace(L"Door003", std::make_unique<DoorParams>(EventType::DOOR, Coord{ 4, 8 }, DoorType::RED));
			tempEventParams[0].emplace(L"StairUp", std::make_unique<StairParams>(EventType::STAIR, Coord{ 6, 5 }, StairType::UP, Coord{ 1, 1 }));
			tempEventParams[0].emplace(L"General001", std::make_unique<GeneralEventParams>(EventType::DEFAULT, Coord{ 6, 6 }, 0, false, ColliderType::TRIGGER));
			tempEventParams[0].emplace(L"Arrow001", std::make_unique<ArrowParams>(EventType::ARROW, Coord{ 6, 7 }, ArrowDirection::DOWN));
			tempEventParams[0].emplace(L"Arrow002", std::make_unique<ArrowParams>(EventType::ARROW, Coord{ 7, 7 }, ArrowDirection::UP));
			tempEventParams[0].emplace(L"Arrow003", std::make_unique<ArrowParams>(EventType::ARROW, Coord{ 8, 7 }, ArrowDirection::LEFT));
			tempEventParams[0].emplace(L"Arrow004", std::make_unique<ArrowParams>(EventType::ARROW, Coord{ 9, 7 }, ArrowDirection::RIGHT));
			tempEventParams[0].emplace(L"Terrain001", std::make_unique<TerrainParams>(EventType::TERRAIN, Coord{ 10, 7 }, TerrainType::BLOCK));

			
			return tempEventParams;
		}();

	for (int floor = 0; floor < 100; floor++)
	{
		std::unordered_map<Coord, UINT> tiles;
		for (int y = 0; y <= gameHeight - 1; y++)
		{
			for (int x = 0; x <= gameWidth - 1; x++)
			{
				// wall of the map
				if (y == 0)
				{
					tiles.insert({ Coord(x, y), 49 });
				}
				else if (x == 0)
				{
					tiles.insert({ Coord(x, y), 40 });
				}
				else if (x == gameWidth - 1)
				{
					tiles.insert({ Coord(x, y), 42 });
				}
				else if (y == gameHeight - 1)
				{
					tiles.insert({ Coord(x, y), 41 });
				}
				else
					tiles.insert({ Coord(x, y), 11 });
				// four corners are different tiles
				tiles[Coord(0, 0)] = 48;
				tiles[Coord(gameWidth - 1, 0)] = 50;
				tiles[Coord(0, gameHeight - 1)] = 40;
				tiles[Coord(gameWidth - 1, gameHeight - 1)] = 42;
			}
		}
		mapTileIdx.insert({ floor, tiles });
	}
	std::unordered_map<int, std::unordered_map<Coord, UINT>> wallTiles =
	{
		{Floor::FLOOR_0,
		{
			{Coord(1,1), 15}, {Coord(1,2), 15}, {Coord(1,3), 15}, {Coord(1,4), 15},
			{Coord(1,5), 15}, {Coord(1,6), 15}, {Coord(1,7), 15}, {Coord(1,8), 15},
			{Coord(1,9), 15}, {Coord(1,10), 15}, {Coord(1,11), 15}, {Coord(2,1), 15},
			{Coord(2,11), 15}, {Coord(3,1), 15}, {Coord(3,11), 15}, {Coord(4,1), 15},
			{Coord(4,11), 15}, {Coord(5,11), 15}, {Coord(6,11), 15} , {Coord(7,11), 15} ,
			{Coord(8,1), 15}, {Coord(8,11), 15}, {Coord(9,1), 15},
			{Coord(9,11), 15}, {Coord(10,1), 15}, {Coord(10,11), 15}, {Coord(11,1), 15},
			{Coord(11,2), 15}, {Coord(11,3), 15}, {Coord(11,4), 15}, {Coord(11,5), 15},
			{Coord(11,6), 15}, {Coord(11,7), 15}, {Coord(11,8), 15}, {Coord(11,9), 15},
			{Coord(11,10), 15}, {Coord(11,11), 15}

		}
		},
		{Floor::FLOOR_1,
		{
			{Coord(1,4), 6}, {Coord(1,8), 6}, {Coord(3,4), 6}, {Coord(3,8), 6}, {Coord(4,4), 6},
			{Coord(4,5), 6}, {Coord(4,6), 6}, {Coord(4,7), 6}, {Coord(4,8), 6}, {Coord(5,4), 6},
			{Coord(5,8), 6}, {Coord(7,4), 6}, {Coord(7,8), 6}, {Coord(8,4), 6}, {Coord(8,5), 6},
			{Coord(8,6), 6}, {Coord(8,7), 6}, {Coord(8,8), 6}, {Coord(9,4), 6}, {Coord(9,8), 6},
			{Coord(11,4), 6}, {Coord(11,8), 6}
		}
		}
	};
	for (const auto& outer_kv : wallTiles)
	{
		auto& inner_map = mapTileIdx[outer_kv.first];
		for (const auto& inner_kv : outer_kv.second)
		{
			inner_map[inner_kv.first] = inner_kv.second;  // update inner_map value
		}
	}
}


std::unordered_map<int, std::map<std::wstring, std::unique_ptr<EventParams>>> MapStatic::eventParams;
std::unordered_map<int, std::unordered_map<Coord, UINT>> MapStatic::mapTileIdx;