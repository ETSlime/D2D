#pragma once

#include "Util.h"
#include "RectBase.h"
#include "Timer.h"

class CursorTextureRect :public RectBase<VertexTexture>
{
public:

	CursorTextureRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, 
		float rotation, std::wstring texturePath);

	~CursorTextureRect();

	void SetVertices() override;
	void SetIndices() override;
	void Update() override;
	void Render() override;

	void SetCursorEnable(bool enable) { shaderData.enabled = enable; }

private:

	struct ShaderData
	{
		float totalTime;
		float enabled;
	};

	UploadBuffer<ShaderData>* shaderBuffer;
	ShaderData shaderData;
	std::vector<VertexTexture> vertices;
	std::vector<UINT> indices;
};

