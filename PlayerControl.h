#pragma once

#include "Animator.h"
#include "Keyboard.h"
#include "Util.h"

#define CHECKMOVE movingUp == false && movingLeft == false && movingRight == false && movingDown == false

static int* checker;

class PlayerControl
{
public:
	PlayerControl();
	~PlayerControl();

	void Update();

	void Idle();
	
	// character control
	void UpdatePosition(DirectX::XMFLOAT3& position);
	void SetMovement(DWORD key, DirectX::XMFLOAT3& position);
	void Move(bool& moveDir, DWORD key, DirectX::XMFLOAT3& position);


	DirectX::XMFLOAT3 targetPosition;
	DirectX::XMFLOAT3 prevPosition;
	float stepDuration = 0.2f;
	std::wstring clipName;

	enum Direction
	{
		Left,
		Right,
		Up,
		Down
	};

	DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& startPoint, const DirectX::XMFLOAT3& endPoint, float t);

	int GetFacingWhere() { return movingWhere; }

	void collision(DirectX::XMFLOAT3* position, float speedx, float speedy);

	void SetAnimator(Animator** animator) { this->animator = animator; }



private:
	class Animator** animator = nullptr;

	Keyboard& keyboard = Keyboard::get_instance();

	bool movingUp = false;
	bool movingLeft = false;
	bool movingRight = false;
	bool movingDown = false;
	bool moving = false;

	float elapsedTime = 0;

	Direction movingWhere = Direction::Down;

	float deltaTime = 0.0f;
};
