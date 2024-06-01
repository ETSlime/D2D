#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"

GameEvent::GameEvent(Coord coord, DirectX::XMFLOAT3 size, EventType type, std::wstring name)
	:eventCoord(coord), eventType(type), eventName(name)
{
	DirectX::XMFLOAT3 position = Map::get_instance().GetPositionFromCoord(eventCoord);
	animRect = new AnimationRect(position, size, eventType);
	animator = new Animator();
}

GameEvent::~GameEvent()
{
	SAFE_DELETE(animRect);
	SAFE_DELETE(animator);
}

void GameEvent::SetupAnimRect(ColliderType type)
{
	animRect->SetAnimation(animator);

	// bounding box
	animRect->UpdateBoundingBox();
	animRect->SetBoundingBoxType(type);

	animRect->SetEvent(this);
}

void GameEvent::SetCoord(Coord newCoord)
{
	eventCoord = newCoord;
}

void GameEvent::UpdatePositionByCoord(Coord newCoord)
{
	DirectX::XMFLOAT3 position = Map::get_instance().GetPositionFromCoord(newCoord);
	animRect->UpdatePosition(position);
}

void GameEvent::SetCollision(float LT_x, float LT_y, float RB_x, float RB_y)
{
	animRect->SetCollisionLT(LT_x, LT_y);
	animRect->SetCollisionRB(RB_x, RB_y);
}

DirectX::XMFLOAT3* GameEvent::GetPosition()
{
	if (animRect)
		return animRect->GetPos();
	else
		return nullptr;
}

BoundingBox* GameEvent::GetBoundingBox()
{
	if (animRect)
		return animRect->GetBox();
	else
		return nullptr;
}