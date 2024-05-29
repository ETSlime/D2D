#pragma once

#include "Weapon.h"

constexpr UINT ATTACK_ANIM_FRAME = 5;
constexpr float ANIM_PLAY_SPEED = 10.0f;

Weapon::Weapon(WeaponType type, UINT ID, DirectX::XMFLOAT3 size) : weaponType(type), weaponID(ID)
{
	std::wstring weaponTexture = IconsPath;
	switch (weaponType)
	{
	case WeaponType::ARMOR:
		weaponTexture += L"armor";
		break;
	case WeaponType::SWORD:
		weaponTexture += L"sword";
	}
	if (ID < 10)
		weaponTexture += L"0";
	weaponTexture += std::to_wstring(ID);
	weaponTexture += L".png";

	if (weaponType == WeaponType::SWORD)
	{
		std::wstring effectTexture;
		if (ID > 5)
			effectTexture = L"006-Weapon01.png";
		else
			effectTexture = L"003-Attack01.png";
		Texture2D* effectTex = new Texture2D(AnimationsPath + effectTexture);
		DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(effectTex->GetWidth(), effectTex->GetHeight());


		// attac anim
		AnimationClip* attack = new AnimationClip(L"Attack", effectTex, ATTACK_ANIM_FRAME, DirectX::XMFLOAT2(0.0f, 0.0f),
			DirectX::XMFLOAT2(texSize.x, 0.5f * texSize.y), 1.0f / ANIM_PLAY_SPEED);

		animRect = new AnimationRect(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), size * 2);
		animator = new Animator();

		animator->SetAnim(attack);
		animator->SetCurrentAnimClip(L"Attack");
		animRect->SetAnimation(animator);
	}
}

void Weapon::UpdateAttackEffect(DirectX::XMFLOAT3 position)
{
	if (animRect)
	{
		animator->Update();
		animRect->UpdatePosition(position);
		animRect->Update();
	}


	//class Box
	//{
	//	int length;
	//	int width;
	//};

	//Box box;
	//box.length = 1;
	//box.width = 1;

}

void Weapon::RenderAttackEffect()
{
	if (animRect)
		animRect->Render();
}

Weapon::~Weapon()
{
	SAFE_DELETE(animator);
	SAFE_DELETE(animRect);
}


