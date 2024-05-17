
#include "Player.h"

Player::Player(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, std::wstring playerTexture)
	:Character(position, size)
{
	Texture2D* playerTex = new Texture2D(CharactersPath + playerTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(playerTex->GetWidth(), playerTex->GetHeight());

	// idle Anim
	AnimationClip* IdleD = new AnimationClip(L"IdleD", playerTex, 1, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.25f), 1.0f / 10.0f);
	AnimationClip* IdleL = new AnimationClip(L"IdleL", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.25f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.5f), 1.0f / 10.0f);
	AnimationClip* IdleR = new AnimationClip(L"IdleR", playerTex, 1, DirectX::XMFLOAT2(0, texSize.x * 0.5f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.75f), 1.0f / 10.f);
	AnimationClip* IdleU = new AnimationClip(L"IdleU", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.75f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y), 1.0f / 10.0f);



	// Walk Anim
	AnimationClip* WalkD = new AnimationClip(L"WalkD", playerTex, 4, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.25f), 1.0f / 10.0f);
	AnimationClip* WalkL = new AnimationClip(L"WalkL", playerTex, 4, DirectX::XMFLOAT2(0, texSize.y * 0.25f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.5f), 1.0f / 10.0f);
	AnimationClip* WalkR = new AnimationClip(L"WalkR", playerTex, 4, DirectX::XMFLOAT2(0, texSize.y * 0.5f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.75f), 1.0f / 10.0f);
	AnimationClip* WalkU = new AnimationClip(L"WalkU", playerTex, 4, DirectX::XMFLOAT2(0, texSize.y * 0.75f), DirectX::XMFLOAT2(texSize.x, texSize.y), 1.0f / 10.0f);



	//clip save
	animator->SetAnim(WalkR);
	animator->SetAnim(WalkL);
	animator->SetAnim(WalkU);
	animator->SetAnim(WalkD);
	animator->SetAnim(IdleR);
	animator->SetAnim(IdleL);
	animator->SetAnim(IdleU);
	animator->SetAnim(IdleD);


	animator->SetCurrentAnimClip(L"IdleD");


	animRect->SetAnimation(animator);



	//collision edge
	SetCollision(20, 20, 20, 20);


	SAFE_DELETE(playerTex);
}

Player::~Player()
{


}

void Player::Update()
{

	animator->Update();
	animRect->Update();
	animRect->Move();

	DirectX::XMFLOAT3 tmp = *(animRect->GetPos());
	tmp.y -= 30;


}

void Player::Render()
{
	animRect->Render();
}


