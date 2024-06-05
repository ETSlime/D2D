#pragma once

#include "TextureRect.h"
#include "Animator.h"
#include "PlayerControl.h"
#include "GameEvent.h"


class GameEvent;

class AnimationRect : public TextureRect
{
public:
	AnimationRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, 
		EventType type = EventType::DEFAULT, bool isCentered = false);
	~AnimationRect();

	void Update();
	void Render();

	virtual void Move() override;

	void SetAnimation(Animator* animator) { this->animator = animator; }
	void SetEvent(GameEvent* event) { this->currentEvent = event; }
	void UpdateBoundingBox();
	void SetBoundingBoxType(ColliderType type);
	void SetOnCollision(CollisionCallback onCollision);
	PlayerControl::Direction GetCurDirection() { if (control) return control->movingWhere; }
	BoundingBox* GetBox() { return boundingBox; }
	bool AABB(BoundingBox* other);

	int GetDirection() { return control->GetFacingWhere(); }
	void SetFacingWhere(PlayerControl::Direction dir) { control->SetFacingWhere(dir); }

	void SetclipName(std::wstring clipname1, std::wstring clipname2);



private:
	Animator* animator = nullptr;
	PlayerControl* control = nullptr;
	GameEvent* currentEvent = nullptr;
	ID3D11SamplerState* point[2];
	ID3D11BlendState* bPoint[2];

	std::wstring clipNameL = L"idleL";
	std::wstring clipNameR = L"idleR";



	int _FacingWhere = 0;

	//DirectX::XMFLOAT3 beforePosition = { 0,0,0 };

	//UploadBuffer<>* shaderBuffer = nullptr;

	bool isCollid = false;

	bool isValid = true;

};