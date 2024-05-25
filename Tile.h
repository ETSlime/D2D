#pragma once

#include "Util.h"
#include "DirectHelper.h"
#include "Map.h"

class Tile
{
public:

	Tile(UINT index, const Coord& coord):tileIndex(index), tileCoord(coord)
	{
		if (index > tileXCount * tileYCount - 1)
			assert(false);

		// calculate tile index
		int tileIndex_x = index % tileXCount;
		int tileIndex_y = index / tileXCount;

		float startX = texTileSize.x * tileIndex_x;
		float startY = texTileSize.y * tileIndex_y;

		// UV coordinate
		startUV = DirectX::XMFLOAT2(startX, startY);
		endUV = startUV + texTileSize;

		DirectX::XMFLOAT3 startPosition = Map::get_instance().GetMapStartPosition();
		
		// position
		//position = DirectX::XMFLOAT3(startPosition.x + tileSize.x * coord.x, startPosition.y + tileSize.y * coord.y, 0);
	};

	//DirectX::XMFLOAT3 GetPosition() { return position; }
	//void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }

	DirectX::XMFLOAT2 GetStartUV() { return startUV; }
	void SetStartUV(DirectX::XMFLOAT2 startUV) { this->startUV = startUV; }

	DirectX::XMFLOAT2 GetEndUV() { return endUV; }
	void SetEndUV(DirectX::XMFLOAT2 endUV) { this->endUV = endUV; }

	Coord GetCoord() { return tileCoord; }

	//float GetDistance(Tile* node)
	//{
	//	float distX = abs(position.x - node->position.x);
	//	float distY = abs(position.y - node->position.y);
	//	return distX + distY;
	//}

	bool GetIsWalkable() { return isWalkable; }
	void SetIsWalkable(bool isWalkable) { this->isWalkable = isWalkable; }

	UINT GetIndex() { return tileIndex; }
	void SetIndex(UINT index) { this->tileIndex = index; }

	DirectX::XMFLOAT3 GetSize() { return tileSize; }

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	std::string GetSpriteName() { return spritename; }
	void SetSpriteName(std::string spriteName) { this->spritename = spritename; }



private:
	//DirectX::XMFLOAT3 position = Values::ZeroVec3;
	DirectX::XMFLOAT2 startUV = Values::ZeroVec2;
	DirectX::XMFLOAT2 endUV = Values::ZeroVec2;

	ID3D11ShaderResourceView* srv = nullptr;

	bool isWalkable = true;

	std::string spritename = "";

	UINT tileIndex;

	UINT tileXCount = 8;
	UINT tileYCount = 38;

	DirectX::XMFLOAT3 tileSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);

	Coord tileCoord;

	DirectX::XMFLOAT2 texTileSize = DirectX::XMFLOAT2(1 / (float)tileXCount, 1 / (float)tileYCount);
};
