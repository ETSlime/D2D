#pragma once

#include "Util.h"
#include "AnimationRect.h"
#include "Animator.h"

enum class WeaponType
{
	ARMOR,
	SWORD
};

class Weapon
{
public:
	Weapon(WeaponType type, UINT weaponID, DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Weapon();

	void UpdateAttackEffect(DirectX::XMFLOAT3 position);
	void RenderAttackEffect();
	//void RenderWeaponRect();

	UINT weaponID;
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
	std::wstring name;
	UINT value;
	WeaponType weaponType;
};