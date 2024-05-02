#pragma once
#include "Util.h"

struct RectEdge
{
	DirectX::XMFLOAT3 LT;
	DirectX::XMFLOAT3 RB;
};

class BoundingBox
{
public:
	BoundingBox(RectEdge* edge);
	~BoundingBox();

	bool AABB(BoundingBox* ohter);
	RectEdge* GetEdge() { return edge; }


private:
	RectEdge* edge = nullptr;
};