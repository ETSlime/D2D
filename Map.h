#pragma once

#include "Util.h"
#include "SingletonBase.h"
#include "IGameObj.h"
#include "BoundingBox.h"
#include "MessageDispatcher.h"

typedef void (*BuildMap)();

class Tile;
class MapStatic;
class GameEvent;

class Map:public SingletonBase<Map>
{

public:

	friend class SingletonBase<Map>;
	friend class MapStatic;
	friend class FloorGO;

	Map();
	~Map();

	void GenerateMap(int floor);
	void GenerateEvent(int floor);

	void ClearCurrentMap();
	Tile* GetTile(Coord coord) { return curMap[coord]; }
	UINT GetNumOfTile() { return static_cast<UINT>(curMap.size()); }
	DirectX::XMFLOAT3 GetMapStartPosition() { return mapStartPosition; }
	DirectX::XMFLOAT3 GetPositionFromCoord(Coord coord);

	void UpdateUnwalkableTiles();
	void UpdateCollisionBoxes();

	const std::vector<BoundingBox*>* GetUnwalkableTiles() { return &unwalkableTiles; }
	const std::vector<BoundingBox*>* GetCollisionBoxes() { return &collisionBoxes; }

private:
	std::unordered_map<Coord, Tile*> curMap;
	std::unordered_map<std::wstring, std::shared_ptr<GameEvent>> curEvents;
	std::unordered_map<int, BuildMap> mapBuilder;

	DirectX::XMFLOAT3 mapStartPosition = DirectX::XMFLOAT3(340, 50, 0);

	std::vector<BoundingBox*> unwalkableTiles;
	std::vector<BoundingBox*> collisionBoxes;

	MessageDispatcher& dispatcher = MessageDispatcher::get_instance();

	void UpdateEventsHandler(std::shared_ptr<Message> event);
};

