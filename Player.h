#pragma once

#include "Character.h"


class Player : public Character
{
public:
	Player(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, std::wstring PlayerTexture);
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

private:


};