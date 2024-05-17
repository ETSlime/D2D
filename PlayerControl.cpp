
#include "PlayerControl.h"
#include "Timer.h"
#include "Keyboard.h"

PlayerControl::PlayerControl()
{

}

PlayerControl::~PlayerControl()
{
}

void PlayerControl::Update()
{
	deltaTime += Timer::Delta();
}



void PlayerControl::Idle()
{
	if (movingWhere == Direction::Left)
	{
		(*animator)->SetCurrentAnimClip(L"IdleL");
	}
	else if (movingWhere == Direction::Right)
	{
		(*animator)->SetCurrentAnimClip(L"IdleR");
	}
	else if (movingWhere == Direction::Up)
	{
		(*animator)->SetCurrentAnimClip(L"IdleU");
	}
	else if (movingWhere == Direction::Down)
	{
		(*animator)->SetCurrentAnimClip(L"IdleD");
	}
}


void PlayerControl::SetMovement(DWORD key, DirectX::XMFLOAT3& position)
{
	switch (key)
	{
	case VK_UP:
		movingUp = true;
		movingWhere = Up;
		clipName = L"WalkU";
		targetPosition = DirectX::XMFLOAT3(position.x, position.y + TileHeight, position.z);
		break;
	case VK_DOWN:
		movingDown = true;
		movingWhere = Down;
		clipName = L"WalkD";
		targetPosition = DirectX::XMFLOAT3(position.x, position.y - TileHeight, position.z);
		break;
	case VK_LEFT:
		movingLeft = true;
		movingWhere = Left;
		clipName = L"WalkL";
		targetPosition = DirectX::XMFLOAT3(position.x - TileWidth, position.y, position.z);
		break;
	case VK_RIGHT:
		movingRight = true;
		movingWhere = Right;
		clipName = L"WalkR";
		targetPosition = DirectX::XMFLOAT3(position.x + TileWidth, position.y, position.z);
		break;
	default:
		break;
	}

	prevPosition = position;
}

void PlayerControl::Move(bool& moveDir, DWORD key, DirectX::XMFLOAT3& position)
{
	float t = elapsedTime / stepDuration;
	elapsedTime += Timer::Delta();
	position = Lerp(prevPosition, targetPosition, t);
	(*animator)->SetCurrentAnimClip(clipName);

	if (t >= 1.0f)
	{
		elapsedTime = 0;
		if (keyboard.Press(key))
		{
			switch (key)
			{
			case VK_UP:
				targetPosition = DirectX::XMFLOAT3(position.x, position.y + TileHeight, position.z);
				break;
			case VK_DOWN:
				targetPosition = DirectX::XMFLOAT3(position.x, position.y - TileHeight, position.z);
				break;
			case VK_LEFT:
				targetPosition = DirectX::XMFLOAT3(position.x - TileWidth, position.y, position.z);
				break;
			case VK_RIGHT:
				targetPosition = DirectX::XMFLOAT3(position.x + TileWidth, position.y, position.z);
				break;
			}
			prevPosition = position;
		}
		else
		{
			moveDir = false;
		}
	}
}

void PlayerControl::UpdatePosition(DirectX::XMFLOAT3& position)
{
	if (keyboard.Down(VK_UP) && CHECKMOVE)
	{
		SetMovement(VK_UP, position);
	}
	else if (keyboard.Down(VK_LEFT) && CHECKMOVE)
	{
		SetMovement(VK_LEFT, position);
	}
	else if (keyboard.Down(VK_RIGHT) && CHECKMOVE)
	{
		SetMovement(VK_RIGHT, position);
	}
	else if (keyboard.Down(VK_DOWN) && CHECKMOVE)
	{
		SetMovement(VK_DOWN, position);
	}

	if (movingUp)
	{
		Move(movingUp, VK_UP, position);
	}
	else if (movingLeft)
	{
		Move(movingLeft, VK_LEFT, position);
	}
	else if (movingRight)
	{
		Move(movingRight, VK_RIGHT, position);
	}
	else if (movingDown)
	{
		Move(movingDown, VK_DOWN, position);

	}
	else if (keyboard.Press(VK_UP) && CHECKMOVE)
	{
		SetMovement(VK_UP, position);
	}
	else if (keyboard.Press(VK_DOWN) && CHECKMOVE)
	{
		SetMovement(VK_DOWN, position);
	}
	else if (keyboard.Press(VK_LEFT) && CHECKMOVE)
	{
		SetMovement(VK_LEFT, position);
	}
	else if (keyboard.Press(VK_RIGHT) && CHECKMOVE)
	{
		SetMovement(VK_RIGHT, position);
	}
	else
		Idle();
}

void PlayerControl::collision(DirectX::XMFLOAT3* position, float speedx, float speedy)
{
	(*position).y += speedy * Timer::Delta();
	(*position).x += speedx * Timer::Delta();
}


DirectX::XMFLOAT3 PlayerControl::Lerp(const DirectX::XMFLOAT3& startPoint, const DirectX::XMFLOAT3& endPoint, float t)
{
	DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&startPoint);
	DirectX::XMVECTOR endVec = DirectX::XMLoadFloat3(&endPoint);


	DirectX::XMVECTOR lerpVec = DirectX::XMVectorLerp(startVec, endVec, t);

	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, lerpVec);

	return result;
}