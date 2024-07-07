#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

class GeneralEvent : public GameEvent
{
public:
	GeneralEvent(Coord coord, UINT triggerID, ColliderType type, bool triggerOnce, 
		std::wstring eventName = L"DefaultEventName", DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~GeneralEvent();

	virtual void Update() override;
	virtual void Render() override;
	void OnPlayerCollision(Coroutine& coro);

private:

	ColliderType colliderType;
	bool triggerOnce;
	UINT triggerID;
};