#pragma once
#pragma once

#include "AnimationRect.h"
#include "Animator.h"

class Character
{
public:
	Character(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size);
	virtual ~Character();

	virtual void Update() = 0;
	virtual void Render() = 0;
	DirectX::XMFLOAT3* GetPosition();
	void SetCollision(float LT_x, float LT_y, float RB_x, float RB_y);


protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
};