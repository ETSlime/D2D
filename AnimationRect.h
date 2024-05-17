#pragma once

#include "TextureRect.h"
#include "Animator.h"
#include "PlayerControl.h"

class AnimationRect : public TextureRect
{
public:
	AnimationRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size);
	~AnimationRect();

	void Update();
	void Render();

	virtual void Move() override;

	void SetAnimation(Animator* animator) { this->animator = animator; }

	DirectX::XMFLOAT3 targetpostiont = { 0,0,0 };


	BoundingBox* GetBox() { return box; }
	bool AABB(BoundingBox* other);

	int GetDirection() { return control->GetFacingWhere(); }
	void SetFacingWhere(int a) { this->_FacingWhere = a; }

	void SetclipName(std::wstring clipname1, std::wstring clipname2);



private:
	class Animator* animator = nullptr;
	class PlayerControl* control = nullptr;
	ID3D11SamplerState* point[2];
	ID3D11BlendState* bPoint[2];

	std::wstring clipNameL = L"idleL";
	std::wstring clipNameR = L"idleR";


	float KeyDown = 0.0f;
	float keyRate = 0.0f;
	float deltaTime = 0.0f;
	float Time = 0.0f;


	int _FacingWhere = 0;

	DirectX::XMFLOAT3 beforePosition = { 0,0,0 };

	//UploadBuffer<>* shaderBuffer = nullptr;

	bool isCollid = false;

	bool isValid = true;

};