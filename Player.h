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

	void aaa() { std::cout << animRect->position.x << " " << animRect->position.y; }

private:

};