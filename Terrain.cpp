#include "Terrain.h"
#include "Player.h"
#include "MapStatic.h"

constexpr UINT IDLE_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 5.0f;

void Terrain::OnPlayerCollision(Coroutine& coro)
{
	switch (terrainType)
	{
	case TerrainType::HP:
		mApp.SetShakeEffect(true);
		Player::player->ChangeHP(-50);
		break;
	case TerrainType::BLOCK:
		SetBlocking();
		MapStatic::eventParams[Player::player->GetCurFloor()][eventName] = std::make_unique<TerrainParams>(
			EventType::TERRAIN, eventCoord, TerrainType::BLOCK, blocking);
		break;
	default:
		break;

	}

	coro.setComplete();
};

Terrain::Terrain(Coord coord, TerrainType type, bool isBlocking, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::TERRAIN, eventName), terrainType(type), blocking(isBlocking)
{
	std::wstring terrainTexture;
	terrainTexture = L"terrain";
	switch (terrainType)
	{
	case TerrainType::HP:
		terrainTexture += L"HP.png";
		break;
	case TerrainType::BLOCK:
	{
		terrainTexture += L"Block.png";
		std::wstring blockTexture;
		blockTexture = L"Block.png";
		Texture2D* blockTex = new Texture2D(GameEventsPath + blockTexture, READ_TEXTURE_NONREPEAT);
		DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(blockTex->GetWidth(), blockTex->GetHeight());
		// idle Anim
		AnimationClip* Block = new AnimationClip(L"Block", blockTex, 1,
			DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(texSize.x, texSize.y), 1.0f / 1.0f);
		animator->SetAnim(Block);
		break;
	}
	default:
		break;
	}
	Texture2D* terrainTex = new Texture2D(GameEventsPath + terrainTexture, READ_TEXTURE_NONREPEAT);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(terrainTex->GetWidth(), terrainTex->GetHeight());

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", terrainTex, IDLE_ANIM_FRAME, DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(texSize.x, texSize.y), 1.0f / ANIM_PLAY_SPEED);


	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::TRIGGER);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(terrainTex);
}

Terrain::~Terrain()
{


}

void Terrain::Update()
{
	animator->Update();
	animRect->Update();
	if (blocking)
	{
		animator->SetCurrentAnimClip(L"Block");
		SetupAnimRect(ColliderType::BLOCKING);
	}

}

void Terrain::Render()
{
	animRect->Render();
}
