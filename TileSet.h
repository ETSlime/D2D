#pragma once

#include "SingletonBase.h"
#include "Util.h"
#include "DirectHelper.h"

class TileSet : public SingletonBase<TileSet>
{
public:
	friend class TMap;
	friend class SingletonBase<TileSet>;

	void GUI();
	ID3D11ShaderResourceView* GetSRV() { return tileSRV; }

private:
	TileSet();
	~TileSet();

private:
	ID3D11ShaderResourceView* tileSRV = nullptr;

	UINT tileXCount = 0;
	UINT tileYCount = 0;

	DirectX::XMFLOAT2 selectedStartUV = Values::ZeroVec2;
	DirectX::XMFLOAT2 texelTileSize = Values::ZeroVec2;
};