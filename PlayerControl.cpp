
#include "PlayerControl.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Player.h"

PlayerControl::PlayerControl()
{
	targetPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	prevPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
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
	DirectX::XMFLOAT3 move;
	switch (key)
	{
	case VK_UP:
		move = DirectX::XMFLOAT3(0.0f, TileHeight, 0.0f);
		movingWhere = Up;
		if (!player->CanMove(move))
		{
			Idle();
			return;
		}
		movingUp = true;
		clipName = L"WalkU";
		targetPosition = position + move;
		break;
	case VK_DOWN:
		move = DirectX::XMFLOAT3(0.0f, 0.0f - TileHeight, 0.0f);
		movingWhere = Down;
		if (!player->CanMove(move))
		{
			Idle();
			return;
		}
		movingDown = true;
		clipName = L"WalkD";
		targetPosition = position + move;
		break;
	case VK_LEFT:
		move = DirectX::XMFLOAT3(0.0f - TileWidth, 0.0f, 0.0f);
		movingWhere = Left;
		if (!player->CanMove(move))
		{
			Idle();
			return;
		}
		movingLeft = true;
		clipName = L"WalkL";
		targetPosition = position + move;
		break;
	case VK_RIGHT:
		move = DirectX::XMFLOAT3(TileWidth, 0.0f, 0.0f);
		movingWhere = Right;
		if (!player->CanMove(move))
		{
			Idle();
			return;
		}
		movingRight = true;
		clipName = L"WalkR";
		targetPosition = position + move;
		break;
	default:
		break;
	}
	UpdatePlayerCoord(key);
	prevPosition = position;
}

void PlayerControl::Move(bool& moveDir, DWORD key, DirectX::XMFLOAT3& position)
{
	float t = elapsedTime / stepDuration;
	elapsedTime += Timer::Delta();
	position = Lerp(prevPosition, targetPosition, t);
	(*animator)->SetCurrentAnimClip(clipName);

	if (t >= 0.99f)
	{
		elapsedTime = 0;
		if (keyboard.Press(key) && Player::player->allowControl == true)
		{
			DirectX::XMFLOAT3 move;
			switch (key)
			{
			case VK_UP:
				move = DirectX::XMFLOAT3(0.0f, TileHeight, 0.0f);
				if (!player->CanMove(move))
				{
					moveDir = false;
					player->UpdatePositionByCoord(player->GetCoord());
					Idle();
					return;
				}
				targetPosition = position + move;
				break;
			case VK_DOWN:
				move = DirectX::XMFLOAT3(0.0f, 0.0f - TileHeight, 0.0f);
				if (!player->CanMove(move))
				{
					moveDir = false;
					player->UpdatePositionByCoord(player->GetCoord());
					Idle();
					return;
				}
				targetPosition = position + move;
				break;
			case VK_LEFT:
				move = DirectX::XMFLOAT3(0.0f - TileWidth, 0.0f, 0.0f);
				if (!player->CanMove(move))
				{
					moveDir = false;
					player->UpdatePositionByCoord(player->GetCoord());
					Idle();
					return;
				}
				targetPosition = position + move;
				break;
			case VK_RIGHT:
				move = DirectX::XMFLOAT3(TileWidth, 0.0f, 0.0f);
				if (!player->CanMove(move))
				{
					moveDir = false;
					player->UpdatePositionByCoord(player->GetCoord());
					Idle();
					return;
				}
				targetPosition = position + move;
				break;
			}
			UpdatePlayerCoord(key);
			prevPosition = position;
		}
		else
		{
			player->UpdatePositionByCoord(player->GetCoord());
			moveDir = false;
		}
	}
}

void PlayerControl::UpdatePlayerPosition(DirectX::XMFLOAT3& position)
{
	if (CHECKMOVE && Player::player->allowControl == false)
	{
		Idle();
		return;
	}	

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

void PlayerControl::UpdatePlayerCoord(DWORD key)
{
	Coord coord = player->GetCoord();
	switch (key)
	{
	case VK_UP:
		player->SetCoord(Coord(coord.x, coord.y + 1));
		break;
	case VK_DOWN:
		player->SetCoord(Coord(coord.x, coord.y - 1));
		break;
	case VK_LEFT:
		player->SetCoord(Coord(coord.x - 1, coord.y));
		break;
	case VK_RIGHT:
		player->SetCoord(Coord(coord.x + 1, coord.y));
		break;
	}
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