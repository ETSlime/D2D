#pragma once

#include "Map.h"
#include "IGameObj.h"
#include "Tile.h"

Map::Map()
{
	mapBuilder[0] = MapStatic::BuildFloor0;
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
	// release tile resource
	for (auto& tile : curMap)
		SAFE_DELETE(tile.second);

	curMap.clear();
}

void Map::GenerateMap(int floor)
{
	mapBuilder[floor]();
}

DirectX::XMFLOAT3 Map::GetPositionFromCoord(Coord coord)
{
	return mapStartPosition + DirectX::XMFLOAT3(coord.x * TileWidth, coord.y * TileHeight, 0.0f);
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

// define here to prevent link error
std::unordered_map<Coord, UINT> MapStatic::baseFloor[Map::numFloor];
std::unordered_map<Coord, IGameObj*> MapStatic::eventFloor[Map::numFloor];

void MapStatic::GenerateTileMap(UINT floor)
{
	for (UINT i = 0; i < Map::gameWidth; i++)
	{
		for (UINT j = 0; j < Map::gameHeight; j++)
		{
			Map::get_instance().curMap[Coord(i, j)] = new Tile(baseFloor[floor].at(Coord(i, j)), Coord(i, j));
		}
	}
}

void MapStatic::BuildFloor0()
{
	for (UINT i = 0; i < Map::gameWidth; i++)
	{
		for (UINT j = 0; j < Map::gameHeight; j++)
		{
			baseFloor[0].insert({ Coord(i, j), i + j});
		}
	}

	GenerateTileMap(0);
}

void MapStatic::BuildFloor1()
{
	baseFloor[0] =
	{
		{Coord(0,0), 0},
		{Coord(1,1), 1},
	};
	GenerateTileMap(1);
}

