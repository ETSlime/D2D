#pragma once

#include "Util.h"
#include "VertexTypes.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "DefaultBuffer.h"
#include "UploadBuffer.h"

class UITextureRect
{
public:
	UITextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring path);
	UITextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation = 0.0f); //header에다만 초기화를 시켜주는게 보기좋다.
	virtual ~UITextureRect();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	void SetShader(std::wstring shaderPath);

	void Update();
	void UpdateWorld();
	void Render();


	void changeshaded(int i);


	virtual void Move();

	void GUI();

	std::string MakeLabel(std::string preFix);

	void MapVertexBuffer();
	void UnmapVertexBuffer();

protected:
	std::vector<VertexTexture> vertices;
	DefaultBuffer* mVertexBuffer = nullptr;

	std::vector<UINT> indices;
	DefaultBuffer* mIndexBuffer = nullptr;

	D3D11_MAPPED_SUBRESOURCE subResource;

	VertexShader* mVertexShader = nullptr;
	PixelShader* mPixelShader = nullptr;

	InputLayout* mInputLayout = nullptr;

	DirectX::XMFLOAT4X4 world;

	struct Data
	{
		int Selection;
		DirectX::XMFLOAT3 dummy;
	};

	Data shaderData = { 0,{}};

	UploadBuffer<Data>* mShaderBuffer = nullptr;

	UploadBuffer<DirectX::XMFLOAT4X4>* mWorldBuffer = nullptr;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	float rotation;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};
