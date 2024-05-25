#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

class Player : public GameEvent
{
public:
	Player(Coord coord, DirectX::XMFLOAT3 size, std::wstring PlayerTexture);
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

	bool CanMove(const DirectX::XMFLOAT3& move);

private:

	const std::vector<BoundingBox*>* unwalkableTiles = Map::get_instance().GetUnwalkableTiles();

};