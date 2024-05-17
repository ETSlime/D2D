#pragma once

#include "Tile.h"
#include "Texture2D.h"
#include "VertexTypes.h"
#include "RectBase.h"
#include "Map.h"



class TMap:public RectBase<VertexTile>
{
public:
	TMap(int floor);
	~TMap();

	void Update() override;
	void Render() override;
	void SetVertices() override;
	void SetIndices() override;

	void MapVertexBuffer();
	void UnmapVertexBuffer();

private:
	UINT tileXCount = 8;
	UINT tileYCount = 38;


	std::vector<VertexTile> vertices;

	std::vector<UINT> indices;
	UINT tileIndex;
	UploadBuffer<UINT>* tileIndexBuffer = nullptr;

	D3D11_MAPPED_SUBRESOURCE subResource;

	Map& map = Map::get_instance();
	ID3D11ShaderResourceView* tileSRV = nullptr;
};

