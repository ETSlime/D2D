#pragma once
#include "Util.h"

enum class ColliderType
{
	BLOCKING, 
	TRIGGER
};

struct RectEdge
{
	DirectX::XMFLOAT3 LT;
	DirectX::XMFLOAT3 RB;

	RectEdge(DirectX::XMFLOAT3 LT, DirectX::XMFLOAT3 RB) :LT(LT), RB(RB) {}
	RectEdge() = default;
};

// function pointer for collison
using CollisionCallback = std::function<void()>;

class BoundingBox
{
public:
	BoundingBox(RectEdge* edge, ColliderType type = ColliderType::BLOCKING);
	~BoundingBox();

	bool AABB(BoundingBox* ohter);
	RectEdge* GetEdge() { return edge; }
	void SetEdge(DirectX::XMFLOAT3 newLT, DirectX::XMFLOAT3 newRB);
	void handleCollision();

	ColliderType colliderType;
	CollisionCallback onCollision;

private:
	RectEdge* edge = nullptr;
};