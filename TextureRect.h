#pragma once

#include "Util.h"
#include "DefaultBuffer.h"
#include "UploadBuffer.h"
#include "VertexTypes.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "BoundingBox.h"
#include "InputLayout.h"

class TextureRect
{
public:
	TextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring path);
	TextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation = 0.0f);
	virtual ~TextureRect();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	void SetShader(std::wstring shaderPath);

	void Update();
	void UpdateWorld();
	void Render();
	virtual void Move();

	void GUI();

	std::string MakeLabel(std::string preFix);

	void MapVertexBuffer();
	void UnmapVertexBuffer();

	//��ġ��E��Լ�E
	DirectX::XMFLOAT3* GetPos() { return &position; }
	void SetPos(DirectX::XMFLOAT3 Pos) { this->position = Pos; }

	//�浹�氁E�����Լ�E(�⺻ vertex�� ��ġ�� �����ϴ� �κ�)
	void SetCollisionLT(float x, float y) { this->CollisionEdge_LT = { x,y }; }
	void SetCollisionRB(float x, float y) { this->CollisionEdge_RB = { x,y }; }



protected:

	//���۵�E������E� ���ؽ�����E�ε��� ���� ����E��ҽ� ���ؽ� ���̴�E�ȼ� ���̴�E��ǲ���̾ƿ������� �Ǿ��ִ�.
	std::vector<VertexTexture> vertices;
	DefaultBuffer* mVertexBuffer = nullptr;

	std::vector<UINT> indices;
	DefaultBuffer* mIndexBuffer = nullptr;

	D3D11_MAPPED_SUBRESOURCE subResource;

	VertexShader* mVertexShader = nullptr;
	PixelShader* mPixelShader = nullptr;

	InputLayout* mInputLayout = nullptr;


	DirectX::XMFLOAT4X4 world;

	UploadBuffer<DirectX::XMFLOAT4X4>* mWorldBuffer = nullptr;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	float rotation;


	//���̴�E���ҽ� ��E
	ID3D11ShaderResourceView* srv = nullptr;




	//�浹 ��ǁE������E
	RectEdge* edge = nullptr;
	BoundingBox* box = nullptr;


	//edge�� �̹����� �°� �浹�������ִ� ����E
	DirectX::XMFLOAT2 CollisionEdge_LT = { 0,0 };
	DirectX::XMFLOAT2 CollisionEdge_RB = { 0,0 };

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

};