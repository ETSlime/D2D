#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

enum class ItemCategory
{
	KEYS,
	AUXILIARIES,
	TREASURES,
};


class Item : public GameEvent
{
public:
	Item(Coord coord, ItemID id, std::wstring eventName = L"DefaultEventName",
		DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Item();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }
	void OnPlayerCollision(Coroutine& coro);

private:

	ItemID itemID;

};