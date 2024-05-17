#include "TileSet.h"
#include "Texture2D.h"

void TileSet::GUI()
{
	ImGui::Begin("TileSet");
	{
		int count = 0;
		for (UINT y = 0; y < tileYCount; y++)
		{
			for (UINT x = 0; x < tileXCount; x++)
			{
				float startX = texelTileSize.x * x;
				float startY = texelTileSize.y * y;

				ImGui::PushID(count);
				if (ImGui::ImageButton(tileSRV, ImVec2(40, 40), ImVec2(startX, startY), ImVec2(startX + texelTileSize.x, startY + texelTileSize.y)))
				{
					selectedStartUV = DirectX::XMFLOAT2(startX, startY);
				}
				count++;
				ImGui::PopID();
				if (count % 4 != 0)
					ImGui::SameLine();
			}//for(x)
		}//for(y)
	}//Begin
	ImGui::End();
}

TileSet::TileSet()
{
	Texture2D* tex = new Texture2D(TilesetsPath + L"tiles.png");
	tileSRV = tex->GetSRV();
	SAFE_DELETE(tex);

	tileXCount = 8;
	tileYCount = 38;

	texelTileSize = DirectX::XMFLOAT2(1 / (float)tileXCount, 1 / (float)tileYCount);
}

TileSet::~TileSet()
{
}
