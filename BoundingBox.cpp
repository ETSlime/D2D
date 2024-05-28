#include "BoundingBox.h"

BoundingBox::BoundingBox(RectEdge* edge, ColliderType type)
	:edge(edge), colliderType(type), onCollision(nullptr)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::SetEdge(DirectX::XMFLOAT3 newLT, DirectX::XMFLOAT3 newRB)
{
	edge->LT = newLT;
	edge->RB = newRB;
}

void BoundingBox::handleCollision(Coroutine& coro)
{
	if (onCollision) 
	{
		coro.setCallback(onCollision);
		coro();
	}
}

bool BoundingBox::AABB(BoundingBox* ohter)
{
	if (edge == nullptr || ohter == nullptr) return false;

	RectEdge* otherEdge = ohter->edge;

	if (edge->RB.x > otherEdge->LT.x &&
		edge->LT.x < otherEdge->RB.x &&
		edge->RB.y < otherEdge->LT.y &&
		edge->LT.y > otherEdge->RB.y)
	{
		return true;
	}
		
	if (edge->RB.x == otherEdge->RB.x &&
		edge->LT.x == otherEdge->LT.x &&
		edge->RB.y == otherEdge->RB.y &&
		edge->LT.y == otherEdge->LT.y)
	{
		return true;
	}

	return false;
}
