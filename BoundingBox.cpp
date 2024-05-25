#include "BoundingBox.h"

BoundingBox::BoundingBox(RectEdge* edge)
	:edge(edge)
{
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::AABB(BoundingBox* ohter)
{
	if (edge == nullptr || ohter == nullptr) return false;

	RectEdge* otherEdge = ohter->edge;

	if (edge->RB.x > otherEdge->LT.x &&
		edge->LT.x < otherEdge->RB.x &&
		edge->RB.y < otherEdge->LT.y &&
		edge->LT.y > otherEdge->RB.y)
		return true;

	return false;
}
