#pragma once

#include "PlayerGO.h"


void PlayerGO::Init()
{
	player = new Player(playerCoord, L"002-Braver01.png");
	Player::player = player;
}

void PlayerGO::Update()
{
	player->Update();
}

void PlayerGO::Render()
{
	player->Render();
}

void PlayerGO::PostRender()
{
}

void PlayerGO::Destroy()
{
	SAFE_DELETE(player);

}

void PlayerGO::GUI()
{
}

bool PlayerGO::IsValid()
{
	return valid;
}

bool PlayerGO::IsDestroyed()
{
	return destroy;
}

void PlayerGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void PlayerGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}