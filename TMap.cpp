#include "TMap.h"

TMap::TMap(int floor)
{
	map.GenerateMap(floor);

	Texture2D* tex = new Texture2D(TilesetsPath + L"tiles.png", true);
	tileSRV = tex->GetSRV();


	//GenerateTileMap();
	SetVertices();
	SetIndices();

	CreateRenderResource(vertices, indices, ShaderPath + L"VertexTile.hlsl");


	tileIndexBuffer = new UploadBuffer<UINT>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
	tileIndex = 0;

}

void TMap::SetVertices()
{
	//vertices.assign(4, VertexTile());
	//vertices[0].position = Values::ZeroVec3;
	//vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	//vertices[2].position = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	//vertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);

	UpdateVertices();

}

void TMap::SetIndices()
{
	//indices = { 0,1,2,2,1,3 };

	indices.assign(map.GetNumOfTile() * NUM_INDEX_PER_TILE, UINT());
	for (UINT i = 0; i < indices.size(); i += NUM_INDEX_PER_TILE)
	{
		indices[i] = i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;
		indices[i + 1] = 1 + i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;
		indices[i + 2] = 2 + i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;
		indices[i + 3] = 2 + i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;
		indices[i + 4] = 1 + i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;
		indices[i + 5] = 3 + i / NUM_INDEX_PER_TILE * NUM_VERTEX_PER_TILE;

	}
}

TMap::~TMap()
{
	SAFE_DELETE(tileIndexBuffer);
}

void TMap::Update()
{
	UpdateVertices();
	MapVertexBuffer();
	UnmapVertexBuffer();
	map.UpdateUnwalkableTiles();

	// update wolrd
	DirectX::XMFLOAT3 position = map.GetMapStartPosition();
	DirectX::XMMATRIX T, S, W;
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	W = S * T;
	DirectX::XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));
	mWorldBuffer->MapData(mDeviceContext, world);
}

void TMap::Render()
{
	__super::Render();

	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

	mDeviceContext->PSSetShaderResources(0, 1, &tileSRV);

	mDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void TMap::MapVertexBuffer()
{
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TMap::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);
}

void TMap::UpdateVertices()
{
	vertices.clear();

	std::vector<VertexTile> tileVertices;
	tileVertices.assign(NUM_VERTEX_PER_TILE, VertexTile());
	for (UINT y = 0; y < MapStatic::gameHeight; y++)
	{
		for (UINT x = 0; x < MapStatic::gameWidth; x++)
		{
			std::shared_ptr<Tile> tile = map.GetTile(Coord(x, y));
			for (VertexTile& v : tileVertices)
			{
				v.bWalkable = static_cast<float>(tile->GetIsWalkable());
				v.index = static_cast<float>(tile->GetIndex());
			}
			// position
			tileVertices[0].position = DirectX::XMFLOAT3(static_cast<float>(x), static_cast<float>(y), 0.0f);
			tileVertices[1].position = DirectX::XMFLOAT3(0.0f + static_cast<float>(x), 1.0f + static_cast<float>(y), 0.0f);
			tileVertices[2].position = DirectX::XMFLOAT3(1.0f + static_cast<float>(x), 0.0f + static_cast<float>(y), 0.0f);
			tileVertices[3].position = DirectX::XMFLOAT3(1.0f + static_cast<float>(x), 1.0f + static_cast<float>(y), 0.0f);
			
			// UV
			tileVertices[0].uv = DirectX::XMFLOAT2(tile->GetStartUV().x, tile->GetEndUV().y);
			tileVertices[1].uv = tile->GetStartUV();
			tileVertices[2].uv = tile->GetEndUV();
			tileVertices[3].uv = DirectX::XMFLOAT2(tile->GetEndUV().x, tile->GetStartUV().y);

			vertices.insert(vertices.end(), tileVertices.begin(), tileVertices.end());
		}
	}
}

//void TMap::Render()
//{
//	__super::Render();
//
//	//l-value, r-value
//	mDeviceContext->PSSetShaderResources(0, 1, &tileSRV);
//	for (UINT y = 0; y < Map::gameHeight; y++)
//	{
//		for (UINT x = 0; x < Map::gameWidth; x++)
//		{
//			Tile* tile = map.GetTile(Coord(x, y));
//
//			MapVertexBuffer();
//			{
//				for (VertexTile& v : vertices)
//				{
//					//v.color = tile.GetColor();
//					v.bWalkable = static_cast<float>(tile->GetIsWalkable());
//					v.index = tile->GetIndex();
//				}
//
//				vertices[0].uv = DirectX::XMFLOAT2(tile->GetStartUV().x, tile->GetEndUV().y);
//				vertices[1].uv = tile->GetStartUV();
//				vertices[2].uv = tile->GetEndUV();
//				vertices[3].uv = DirectX::XMFLOAT2(tile->GetEndUV().x, tile->GetStartUV().y);
//			}
//			UnmapVertexBuffer();
//			DirectX::XMFLOAT3 position = tile->GetPosition();
//			DirectX::XMMATRIX T, S, W;
//			T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//			DirectX::XMFLOAT3 size = tile->GetSize();
//			S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
//			W = S * T;
//			DirectX::XMFLOAT4X4 world;
//			XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));
//
//			mWorldBuffer->MapData(mDeviceContext, world);
//			mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());
//
//			tileIndexBuffer->MapData(mDeviceContext, tileIndex);
//			mDeviceContext->PSSetConstantBuffers(0, 1, tileIndexBuffer->Resource());
//
//			mDeviceContext->DrawIndexed(indexCount, 0, 0);
//		}
//	}
//}






