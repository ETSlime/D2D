#pragma once

#include "Util.h"
#include "SingletonBase.h"
#include "Tile.h"
#include "IGameObj.h"

typedef void (*BuildMap)();


class Map:public SingletonBase<Map>
{

public:

	friend class SingletonBase<Map>;
	friend class MapStatic;

	Map();
	~Map();

	void GenerateMap(int floor);
	Tile* GetTile(Coord coord) { return map[coord]; }

	static const UINT gameWidth = 13;
	static const UINT gameHeight = 13;

	static const UINT numFloor = 100;

private:
	std::unordered_map<Coord, Tile*> map;
	std::unordered_map<int, BuildMap> mapBuilder;
};

class MapStatic
{
public:
	// Coord: map coordinate
	// UINT: tile Index
	static std::unordered_map<Coord, UINT> baseFloor[Map::numFloor];
	static std::unordered_map<Coord, IGameObj*> EventFloor[Map::numFloor];

	static void BuildFloor0();
	static void BuildFloor1();


	static void GenerateTileMap(UINT floor);

};