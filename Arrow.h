#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"


class Arrow : public GameEvent
{
public:
	Arrow(Coord coord, ArrowDirection dir, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Arrow();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }
	void OnPlayerCollision(Coroutine& coro, ArrowDirection dir, Coord coord = Coord(0, 0));

private:

	ArrowDirection arrowDir;

};