#pragma once

#include "Util.h"
#include "BoundingBox.h"
#include "RectBase.h"

class TextureRect:public RectBase<VertexTexture>
{
public:
	TextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, bool isCentered = false);

	virtual ~TextureRect();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	void Update() override;
	void Render() override;
	virtual void SetVertices() override;
	virtual void SetIndices() override;

	virtual void Move();

	void GUI();

	std::string MakeLabel(std::string preFix);

	void MapVertexBuffer();
	void UnmapVertexBuffer();

	DirectX::XMFLOAT3* GetPos() { return &position; }
	void SetEnable(bool enable) { shaderData.enabled = enable; }
	void StartFadeIn() { shaderData.fadeOut = false; shaderData.fadeIn = true; shaderData.startTime = Timer::TotalTime(); }
	void StartFadeOut() { shaderData.fadeIn = false; shaderData.fadeOut = true; shaderData.startTime = Timer::TotalTime(); }
	void StopFade() { shaderData.fadeOut = false; shaderData.fadeIn = false; }
	void SetPos(DirectX::XMFLOAT3 Pos) { this->position = Pos; }

	void SetCollisionLT(float x, float y) { this->CollisionEdge_LT = { x,y }; }
	void SetCollisionRB(float x, float y) { this->CollisionEdge_RB = { x,y }; }



protected:

	struct ShaderData
	{
		float enabled;
		float fadeIn;
		float fadeOut;
		float totalTime;
		float startTime;
	};

	std::vector<VertexTexture> vertices;

	std::vector<UINT> indices;
	ShaderData shaderData;
	D3D11_MAPPED_SUBRESOURCE subResource;

	BoundingBox* boundingBox = nullptr;

	UploadBuffer<ShaderData>* shaderBuffer;


	DirectX::XMFLOAT2 CollisionEdge_LT = { 0,0 };
	DirectX::XMFLOAT2 CollisionEdge_RB = { 0,0 };

	bool centered;
};