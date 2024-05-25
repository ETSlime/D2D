#pragma once
#include "Util.h"

struct RectEdge
{
	DirectX::XMFLOAT3 LT;
	DirectX::XMFLOAT3 RB;

	RectEdge(DirectX::XMFLOAT3 LT, DirectX::XMFLOAT3 RB) :LT(LT), RB(RB) {}
	RectEdge() = default;
};

class BoundingBox
{
public:
	BoundingBox(RectEdge* edge);
	~BoundingBox();

	bool AABB(BoundingBox* ohter);
	RectEdge* GetEdge() { return edge; }
	void SetEdge(RectEdge* newEdge) { edge = newEdge; }

private:
	RectEdge* edge = nullptr;
};