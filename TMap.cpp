#include "TMap.h"

TMap::TMap(int floor)
{
	map.GenerateMap(floor);

	Texture2D* tex = new Texture2D(TilesetsPath + L"tiles.png");
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
	vertices.assign(4, VertexTile());
	vertices[0].position = Values::ZeroVec3;
	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[2].position = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
}

void TMap::SetIndices()
{
	indices = { 0,1,2,2,1,3 };
}

TMap::~TMap()
{
	//TileSet::get_instance().~TileSet();

	//for (UINT y = 0; y < height; y++)
	//	SAFE_DELETE_ARRAY(tiles[y]);
	//SAFE_DELETE_ARRAY(tiles);

	SAFE_DELETE(tileIndexBuffer);

}

void TMap::Update()
{
	//Vector3 mPos = Mouse::Get()->GetPosition();
	//Tile* tile = GetTile(mPos);
	//if (tile != nullptr)
	//{
	//	inb->SetIndex(tile->GetIndex());
	//}

	//if (Mouse::Get()->Press(0))
	//{
	//	if (tile != nullptr)
	//	{
	//		tile->SetColor(Values::Red);
	//		Vector2 startUV = TileSet::Get()->selectedStartUV;
	//		Vector2 endUV = startUV + TileSet::Get()->texelTileSize;
	//		tile->SetStartUV(TileSet::Get()->selectedStartUV);
	//		tile->SetEndUV(endUV);
	//		if (mode == RedCell)
	//		{
	//			tile->SetIsWalkable(false);
	//		}
	//	}
	//}

	//if (Keyboard::get_instance().Down(VK_SPACE))
	//{
	//	Build();
	//}
}

void TMap::Render()
{
	__super::Render();

	//l-value, r-value
	mDeviceContext->PSSetShaderResources(0, 1, &tileSRV);
	for (UINT y = 0; y < Map::gameHeight; y++)
	{
		for (UINT x = 0; x < Map::gameWidth; x++)
		{
			Tile* tile = map.GetTile(Coord(x, y));

			MapVertexBuffer();
			{
				for (VertexTile& v : vertices)
				{
					//v.color = tile.GetColor();
					v.bWalkable = tile->GetIsWalkable();
					v.index = tile->GetIndex();
				}

				vertices[0].uv = DirectX::XMFLOAT2(tile->GetStartUV().x, tile->GetEndUV().y);
				vertices[1].uv = tile->GetStartUV();
				vertices[2].uv = tile->GetEndUV();
				vertices[3].uv = DirectX::XMFLOAT2(tile->GetEndUV().x, tile->GetStartUV().y);

				vertices[0].uv2 = DirectX::XMFLOAT2(0, 1);
				vertices[1].uv2 = DirectX::XMFLOAT2(0, 0);
				vertices[2].uv2 = DirectX::XMFLOAT2(1, 1);
				vertices[3].uv2 = DirectX::XMFLOAT2(1, 0);
			}
			UnmapVertexBuffer();
			DirectX::XMFLOAT3 position = tile->GetPosition();
			DirectX::XMMATRIX T, S, W;
			T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			DirectX::XMFLOAT3 size = tile->GetSize();
			S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
			W = S * T;
			DirectX::XMFLOAT4X4 world;
			XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));

			mWorldBuffer->MapData(mDeviceContext, world);
			mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

			tileIndexBuffer->MapData(mDeviceContext, tileIndex);
			mDeviceContext->PSSetConstantBuffers(0, 1, tileIndexBuffer->Resource());

			mDeviceContext->DrawIndexed(indexCount, 0, 0);
		}
	}
}

//void TMap::GUI()
//{
//	mTileSet.GUI();
//
//
//	ImGui::Begin("TileMap");
//	{
//		ImGui::RadioButton("Normal", (int*)&mode, 0);
//		ImGui::RadioButton("RedCell", (int*)&mode, 1);
//	}
//	ImGui::End();
//}

//void TMap::GenerateTileMap()
//{
//	if (width == 0 || height == 0 || spacing == 0)
//		assert(false);
//
//	tiles = new Tile * [height];
//	int index = 0;
//	for (UINT y = 0; y < height; y++)
//	{
//		tiles[y] = new Tile[width];
//		for (UINT x = 0; x < width; x++)
//		{
//			tiles[y][x].SetPosition(DirectX::XMFLOAT3((float)(x * spacing), (float)(y * spacing), 0.0f));
//			tiles[y][x].SetIndex(index++);
//		}
//	}
//}

void TMap::MapVertexBuffer()
{
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TMap::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);
}

//Tile* TMap::GetTile(DirectX::XMFLOAT3 mPos)
//{
//	UINT x = (int)mPos.x / spacing;
//	UINT y = (int)mPos.y / spacing;
//
//	if (x < 0 || x >= width || y < 0 || y >= height)
//		return nullptr;
//
//	return &tiles[y][x];
//}


