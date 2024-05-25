#include "GameEvent.h"
#include "AnimationRect.h"

GameEvent::GameEvent(Coord coord, DirectX::XMFLOAT3 size, EventType type)
	:eventCoord(coord), eventType(type)
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
	return animRect->GetPos();
}