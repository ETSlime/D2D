#pragma once

#include "IGameObj.h"

class TextureDemo : public IGameObj
{
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	virtual bool IsValid() override;
	virtual void SetIsValid(bool _valid) override;

	TextureDemo(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	class TextureRect* tr = nullptr;
	class UI* startUI = nullptr;


	bool Valid = true;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};