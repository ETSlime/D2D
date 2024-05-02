#pragma once
#include "Util.h"
#include "BoundingBox.h"
#include "DefaultBuffer.h"
#include "InputLayout.h"
#include "UploadBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexTypes.h"

class Rect
{
public:
	Rect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation);
	~Rect();

	void Update();
	void Render();

	void Move();

	void GUI();

	void UpdateWorld();
	void SetColor(DirectX::XMFLOAT4 color)
	{
		this->color = color;
		UpdateColor();
	}

	BoundingBox* GetBox() { return box; }
	bool AABB(BoundingBox* other) { return box->AABB(other); }

	void UpdateColor();
	std::string MakeLable(std::string preFix);

private:
	std::vector<VertexColor> vertices;
	DefaultBuffer* mVertexBuffer = nullptr;

	std::vector<UINT> indices;
	DefaultBuffer* mIndexBuffer = nullptr;

	InputLayout* mInputLayout = nullptr;

	VertexShader* mVertexShader = nullptr;
	PixelShader* mPixelShader = nullptr;

	DirectX::XMFLOAT4X4 world;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	float rotation;

	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 0, 0, 1);

	UploadBuffer<DirectX::XMFLOAT4X4>* mWorldBuffer = nullptr;

	bool bOpen = true;

	RectEdge* edge = nullptr;
	BoundingBox* box = nullptr;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};