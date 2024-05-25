#pragma once

#include "Util.h"
#include "SingletonBase.h"
#include "IGameObj.h"

typedef void (*BuildMap)();

class Tile;

class Map:public SingletonBase<Map>
{

public:

	friend class SingletonBase<Map>;
	friend class MapStatic;

	Map();
	~Map();

	void GenerateMap(int floor);
	Tile* GetTile(Coord coord) { return curMap[coord]; }
	UINT GetNumOfTile() { return curMap.size(); }
	DirectX::XMFLOAT3 GetMapStartPosition() { return mapStartPosition; }
	DirectX::XMFLOAT3 GetPositionFromCoord(Coord coord) 
	{
		return mapStartPosition + DirectX::XMFLOAT3(coord.x * TileWidth, coord.y * TileHeight, 0.0f);
	}

	static const UINT gameWidth = 13;
	static const UINT gameHeight = 13;

	static const UINT numFloor = 100;

private:
	std::unordered_map<Coord, Tile*> curMap;
	std::unordered_map<int, BuildMap> mapBuilder;

	DirectX::XMFLOAT3 mapStartPosition = DirectX::XMFLOAT3(340, 50, 0);
};

class MapStatic
{
public:
	// Coord: map coordinate
	// UINT: tile Index
	static std::unordered_map<Coord, UINT> baseFloor[Map::numFloor];
	static std::unordered_map<Coord, IGameObj*> eventFloor[Map::numFloor];

	static void BuildFloor0();
	static void BuildFloor1();


	static void GenerateTileMap(UINT floor);

};