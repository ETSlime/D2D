#include "Monster.h"


Monster::Monster(Coord coord, UINT monsterID, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::MONSTER, eventName)
{
	std::wstring MonsterTexture;
	MonsterTexture = L"Monster";
	if (monsterID / 4 + 1 < 10)
		MonsterTexture += L"0";
	MonsterTexture += std::to_wstring(1 + monsterID / 4);
	MonsterTexture += L".png";
	Texture2D* MonsterTex = new Texture2D(GameEventsPath + MonsterTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(MonsterTex->GetWidth(), MonsterTex->GetHeight());

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", MonsterTex, 4, DirectX::XMFLOAT2(0, monsterID % 4 * 0.25f * texSize.y), DirectX::XMFLOAT2(texSize.x, (monsterID % 4 + 1) * 0.25f * texSize.y), 1.0f / 10.0f);



	//clip save
	animator->SetAnim(Idle);


	animator->SetCurrentAnimClip(L"Idle");


	animRect->SetAnimation(animator);
	animRect->SetEvent(this);


	//collision edge
	SetCollision(20, 20, 20, 20);


	SAFE_DELETE(MonsterTex);
}

Monster::~Monster()
{


}

void Monster::Update()
{

	animator->Update();
	animRect->Update();
	animRect->Move();
}

void Monster::Render()
{
	animRect->Render();
}

