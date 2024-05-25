#include "Player.h"


Player::Player(Coord coord, DirectX::XMFLOAT3 size, std::wstring playerTexture)
	:GameEvent(coord, size, EventType::PLAYER)
{
	Texture2D* playerTex = new Texture2D(GameEventsPath + playerTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(playerTex->GetWidth(), playerTex->GetHeight());

	// idle Anim
	AnimationClip* IdleD = new AnimationClip(L"IdleD", playerTex, 1, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.25f), 1.0f / 10.0f);
	AnimationClip* IdleL = new AnimationClip(L"IdleL", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.25f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.5f), 1.0f / 10.0f);
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
	animRect->SetEvent(this);


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

	Map::get_instance().UpdateUnwalkableTiles();
}

void Player::Render()
{
	animRect->Render();
}

bool Player::CanMove(const DirectX::XMFLOAT3& move)
{
	if (Keyboard::get_instance().Press(VK_CONTROL))
		return true;
	BoundingBox* predictedBox = new BoundingBox(*animRect->GetBox());
	predictedBox->SetEdge(new RectEdge(
		predictedBox->GetEdge()->LT + move,
		predictedBox->GetEdge()->RB + move
		));
	for (const auto& unwalkableTile : *(unwalkableTiles)) 
	{
		if (unwalkableTile->AABB(predictedBox))
		{
			SAFE_DELETE(predictedBox);
			return false;
			break;
		}
	}
	SAFE_DELETE(predictedBox);
	return true;
}

