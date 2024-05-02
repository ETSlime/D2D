#pragma once

#include "TextureRect.h"

class UIalphabet;



class UIRect : public TextureRect
{
public:
	UIRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size);
	~UIRect();

	void Update();
	void Render();

	virtual void Move() override;

	void SetAnimation(UIalphabet* alphabet) { this->alphabet = alphabet; }



private:
	class UIalphabet* alphabet = nullptr;
	class PControl* control = nullptr;
	ID3D11SamplerState* point[2];
	ID3D11BlendState* bPoint[2];
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;



	float KeyDown = 0.0f;
	float keyRate = 0.0f;
	float deltaTime = 0.0f;
	float Time = 0.0f;


};