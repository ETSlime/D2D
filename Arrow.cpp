#include "Arrow.h"
#include "Player.h"

void Arrow::OnPlayerCollision(Coroutine& coro, ArrowDirection dir, Coord coord)
{
	switch (dir)
	{
	case ArrowDirection::UP:
		// prevent from walking from up
		if (coord == Coord(eventCoord.x, eventCoord.y + 1))
			SetupAnimRect(ColliderType::BLOCKING);
		else
		{
			SetupAnimRect(ColliderType::TRIGGER);
			Player::player->SewWalkable({false, false, true, false});
		}
		break;
	case ArrowDirection::DOWN:
		// prevent from walking from down
		if (coord == Coord(eventCoord.x, eventCoord.y - 1))
			SetupAnimRect(ColliderType::BLOCKING);
		else
		{
			SetupAnimRect(ColliderType::TRIGGER);
			Player::player->SewWalkable({ false, false, false, true });
		}
		break;
	case ArrowDirection::LEFT:
		// prevent from walking from left
		if (coord == Coord(eventCoord.x - 1, eventCoord.y))
			SetupAnimRect(ColliderType::BLOCKING);
		else
		{
			SetupAnimRect(ColliderType::TRIGGER);
			Player::player->SewWalkable({ true, false, false, false });
		}
		break;
	case ArrowDirection::RIGHT:
		// prevent from walking from right
		if (coord == Coord(eventCoord.x + 1, eventCoord.y))
			SetupAnimRect(ColliderType::BLOCKING);
		else
		{
			SetupAnimRect(ColliderType::TRIGGER);
			Player::player->SewWalkable({ false, true, false, false });
		}
		break;
	default:
		break;
	}

	coro.setComplete();
};


Arrow::Arrow(Coord coord, ArrowDirection dir, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::ARROW, eventName), arrowDir(dir)
{
	std::wstring arrowTexture = L"arrow.png";
	Texture2D* arrowTex = new Texture2D(GameEventsPath + arrowTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(arrowTex->GetWidth(), arrowTex->GetHeight());

	SetupAnimRect(ColliderType::TRIGGER);

	float texStartX = 0.0f;
	switch (arrowDir)
	{
	case ArrowDirection::UP:
		// lamda expression  to capture current object and bind its member functions as callbacks.
		animRect->SetOnCollision([this](Coroutine& coro, ArrowDirection dir = ArrowDirection::UP, Coord coord = Player::player->GetCoord())
			{ this->OnPlayerCollision(coro, dir, coord); });
		break;
	case ArrowDirection::DOWN:
		animRect->SetOnCollision([this](Coroutine& coro, ArrowDirection dir = ArrowDirection::DOWN, Coord coord = Player::player->GetCoord())
			{ this->OnPlayerCollision(coro, dir, coord); });
		texStartX = 0.25f * texSize.x;
		break;
	case ArrowDirection::LEFT:
		animRect->SetOnCollision([this](Coroutine& coro, ArrowDirection dir = ArrowDirection::LEFT, Coord coord = Player::player->GetCoord())
			{ this->OnPlayerCollision(coro, dir, coord); });
		texStartX = 0.5f * texSize.x;
		break;
	case ArrowDirection::RIGHT:
		animRect->SetOnCollision([this](Coroutine& coro, ArrowDirection dir = ArrowDirection::RIGHT, Coord coord = Player::player->GetCoord())
			{ this->OnPlayerCollision(coro, dir, coord); });
		texStartX = 0.75f * texSize.x;
		break;
	default:
		break;
	}

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", arrowTex, 1,
		DirectX::XMFLOAT2(texStartX, 0), DirectX::XMFLOAT2(texStartX + 0.25f * texSize.x, texSize.y), 1.0f / 1.0f);
	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SAFE_DELETE(arrowTex);
}

Arrow::~Arrow()
{

}

void Arrow::Update()
{
	animRect->Update();
}

void Arrow::Render()
{
	animRect->Render();
}
