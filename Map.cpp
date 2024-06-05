#pragma once

#include "Map.h"
#include "MapStatic.h"
#include "Tile.h"
#include "EventFactory.h"

void Map::UpdateEventsHandler(std::shared_ptr<Message> event)
{
	auto updateEvent = std::dynamic_pointer_cast<MessageEventUpdate>(event);
	if (updateEvent && curEvents.find(updateEvent->eventName) != curEvents.end())
	{
		curEvents.erase(updateEvent->eventName);
	}
}

Map::Map()
{
	dispatcher.registerHandler("UpdateEvents", static_cast<MessageHandler>(
		std::bind(&Map::UpdateEventsHandler, this, std::placeholders::_1)));

	mapBuilder = MapStatic::BuildFloor;
	//mapBuilder[0] = MapStatic::BuildFloor0;
	//mapBuilder[1] = MapStatic::BuildFloor1;
	//mapBuilder[2] = MapStatic::BuildFloor2;
	//mapBuilder[3] = MapStatic::BuildFloor3;
	//mapBuilder[4] = MapStatic::BuildFloor4;
	//mapBuilder[5] = MapStatic::BuildFloor5;
	//mapBuilder[6] = MapStatic::BuildFloor6;
	//mapBuilder[7] = MapStatic::BuildFloor7;
	//mapBuilder[8] = MapStatic::BuildFloor8;
	//mapBuilder[9] = MapStatic::BuildFloor9;
}

Map::~Map()
{
	ClearCurrentMap();
}

void Map::ClearCurrentMap()
{
	// release tile resource
	for (auto& tile : curMap)
		SAFE_DELETE(tile.second);

	curMap.clear();
	curEvents.clear();
}

void Map::GenerateMap(int floorNum)
{
	mapBuilder(floorNum);
}

void Map::GenerateEvent(int floor)
{
	for (auto& eventDesc : MapStatic::eventFloor[floor])
	{
		curEvents[eventDesc.get()->eventName] = std::move(EventFactory::CreateGameEvent(*(eventDesc.get())));
	}
	//int size = MapStatic::eventFloor[floor].size();
	//for (int i = size - 1; i >= 0; i--)
	//{
	//	auto eventDesc = MapStatic::eventFloor[floor][i].get();
	//	curEvents[eventDesc->eventName] = std::move(EventFactory::CreateGameEvent(*(eventDesc)));
	//}
	
}

DirectX::XMFLOAT3 Map::GetPositionFromCoord(Coord coord)
{
	return mapStartPosition + DirectX::XMFLOAT3(coord.x * static_cast<float>(TileWidth), coord.y * static_cast<float>(TileHeight), 0.0f);
}

void Map::UpdateUnwalkableTiles()
{
	unwalkableTiles.clear();
	for (const auto& tile : curMap)
	{
		if (!tile.second->GetIsWalkable())
		{
			unwalkableTiles.push_back(tile.second->GetBoundingBox());
		}
	}
}

void Map::UpdateCollisionBoxes()
{
	collisionBoxes.clear();
	for (const auto& event : curEvents)
	{
		if (event.second)
			collisionBoxes.push_back(event.second.get()->GetBoundingBox());
	}
}