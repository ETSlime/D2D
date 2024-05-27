#pragma once

#include "Util.h"

struct VertexColor
{
	VertexColor() : position(0, 0, 0), color(DirectX::XMFLOAT4(0, 0, 0, 0)) {}
	VertexColor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color)
		: position(position), color(color)
	{

	}
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;
};

struct VertexTexture
{
	VertexTexture() : position(0, 0, 0), uv(0, 0) {}
	VertexTexture(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv)
		: position(position), uv(uv)
	{

	}
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;
};

struct VertexTextureColor
{
	VertexTextureColor() : position(0, 0, 0), uv(0, 0), color(0, 0, 0, 0) {}
	VertexTextureColor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT4 color)
		: position(position), uv(uv), color(color)
	{

	}
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 3;
};

struct VertexTile
{
	VertexTile() : position(0, 0, 0), uv(0, 0), index(0), bWalkable(0){}
	VertexTile(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv, 
		float index, float walkalbe = 1.0f)
		: position(position), uv(uv), index(index), bWalkable(walkalbe)
	{

	}
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	float index;
	float bWalkable;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 6;
};
