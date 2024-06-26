#include "GameNPC.h"

constexpr UINT IDLE_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 10.0f;

void GameNPC::OnPlayerCollision(Coroutine& coro)
{
	mApp.ShowNPCDialogue(dialogueID);
	
	coro.setComplete();
};

GameNPC::GameNPC(Coord coord, UINT GameNPCID, UINT dialogueID, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::NPC, eventName), GameNPCID(GameNPCID), dialogueID(dialogueID)
{
	std::wstring GameNPCTexture;
	GameNPCTexture = L"001-npc";
	if (GameNPCID / 4 + 1 < 10)
		GameNPCTexture += L"0";
	GameNPCTexture += std::to_wstring(1 + GameNPCID / 4);
	GameNPCTexture += L".png";
	Texture2D* GameNPCTex = new Texture2D(GameEventsPath + GameNPCTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(GameNPCTex->GetWidth(), GameNPCTex->GetHeight());

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", GameNPCTex, IDLE_ANIM_FRAME, DirectX::XMFLOAT2(0, GameNPCID % 4 * 0.25f * texSize.y),
		DirectX::XMFLOAT2(texSize.x, (GameNPCID % 4 + 1) * 0.25f * texSize.y), 1.0f / ANIM_PLAY_SPEED);

	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::BLOCKING);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(GameNPCTex);
}

GameNPC::~GameNPC()
{


}

void GameNPC::Update()
{

	animator->Update();
	animRect->Update();
	animRect->Move();
}

void GameNPC::Render()
{
	animRect->Render();
}
