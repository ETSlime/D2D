#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"


class Terrain : public GameEvent
{
public:
	Terrain(Coord coord, TerrainType type, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Terrain();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }
	void OnPlayerCollision(Coroutine& coro);
	void SetBlocking() { if (terrainType == TerrainType::BLOCK) blocking = true; }

private:

	TerrainType terrainType;
	bool blocking = false;
	MagicTowerApp& mApp = MagicTowerApp::get_instance();
};