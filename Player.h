#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

class Player : public GameEvent
{
public:
	Player(Coord coord, std::wstring PlayerTexture, DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

	bool CanMove(const DirectX::XMFLOAT3& move);

private:

	const std::vector<BoundingBox*>* unwalkableTiles = Map::get_instance().GetUnwalkableTiles();
	const std::vector<BoundingBox*>* collisionBoxes = Map::get_instance().GetCollisionBoxes();

};