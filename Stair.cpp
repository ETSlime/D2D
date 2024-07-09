#pragma once

#include "Stair.h"
#include "ChangeMapEffect.h"
#include "MapStatic.h"

void Stair::OnPlayerCollision(Coroutine& coro, StairType type)
{
	if (coro.getState() == 0)
	{
		Player::player->SetAllowControl(false);
		Player::player->PlayFadeEffect(true);
		coro.yield(Player::player->fadeEffect->GetFadeSpeed());
	}
	if (coro.getState() == 1)
	{
		Player::player->SetFacingDirection(PlayerControl::Direction::Down);
		Player::player->SetCoord(newPlayerCoord);
		Player::player->UpdatePositionByCoord(newPlayerCoord);

		if (type == StairType::UP)
			mApp.LoadFloor(floorNumber + 1);
		else if (type == StairType::DOWN)
			mApp.LoadFloor(floorNumber - 1);
		Player::player->GetanimRect()->SetFacingWhere(PlayerControl::Down);
		Player::player->PlayFadeEffect(false);
		Player::player->SetAllowControl(true);
		destroy = true;
		coro.setComplete();
	}
};


Stair::Stair(Coord coord, StairType type, Coord newCoord, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::STAIR, eventName), stairType(type), newPlayerCoord(newCoord)
{
	std::wstring stairTexture = L"stair.png";
	Texture2D* stairTex = new Texture2D(GameEventsPath + stairTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(stairTex->GetWidth(), stairTex->GetHeight());
	float texStartX = 0.0f;
	if (stairType == StairType::UP)
		texStartX = 0.5f * texSize.x;

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", stairTex, 1,
		DirectX::XMFLOAT2(texStartX, 0), DirectX::XMFLOAT2(texStartX + 0.5f * texSize.x, texSize.y), 1.0f / 1.0f);
	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::TRIGGER);

	switch (stairType)
	{
	case (StairType::UP):
		// lamda expression  to capture current object and bind its member functions as callbacks.
		animRect->SetOnCollision([this](Coroutine& coro, StairType type = StairType::UP) 
			{ this->OnPlayerCollision(coro, type); });
		break;
	case (StairType::DOWN):
		// lamda expression  to capture current object and bind its member functions as callbacks.
		animRect->SetOnCollision([this](Coroutine& coro, StairType type = StairType::DOWN) 
			{ this->OnPlayerCollision(coro, type); });
		break;
	}

	SAFE_DELETE(stairTex);
}

Stair::~Stair()
{

}

void Stair::Update()
{
	animRect->Update();
}

void Stair::Render()
{
	animRect->Render();
}
