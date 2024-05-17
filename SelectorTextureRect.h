#pragma once

#include "Util.h"
#include "RectBase.h"
#include "Timer.h"

class SelectorTextureRect :public RectBase<VertexTexture>
{
public:

	SelectorTextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, 
		float rotation, std::wstring texturePath);

	~SelectorTextureRect();

	void SetVertices() override;
	void SetIndices() override;
	void Update() override;
	void Render() override;

private:
	UploadBuffer<float>* shaderBuffer;
	float totalTime;

	std::vector<VertexTexture> vertices;
	std::vector<UINT> indices;
};

