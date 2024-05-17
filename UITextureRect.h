#pragma once

#include "Util.h"
#include "RectBase.h"

class UITextureRect:public RectBase<VertexTexture>
{
public:
	UITextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, 
		 std::wstring texturePath);

	virtual ~UITextureRect();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	void Update();
	virtual void SetVertices() override;
	virtual void SetIndices() override;

	virtual void Move();

	void GUI();

	std::string MakeLabel(std::string preFix);

	void MapVertexBuffer();
	void UnmapVertexBuffer();

protected:
	std::vector<VertexTexture> vertices;
	std::vector<UINT> indices;

	D3D11_MAPPED_SUBRESOURCE subResource;

};
