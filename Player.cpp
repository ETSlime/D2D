#include "Player.h"
#include "AnimationRect.h"
#include "ChangeMapEffect.h"
#include "ButtonOnClick.h"

Player* Player::player = nullptr;

Player::Player(Coord coord, std::wstring playerTexture, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::PLAYER)
{
	Texture2D* playerTex = new Texture2D(GameEventsPath + playerTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(playerTex->GetWidth(), playerTex->GetHeight());

	// idle anim
	AnimationClip* IdleD = new AnimationClip(L"IdleD", playerTex, 1, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.25f), 1.0f / 10.0f);
	AnimationClip* IdleL = new AnimationClip(L"IdleL", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.25f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.5f), 1.0f / 10.0f);
	AnimationClip* IdleR = new AnimationClip(L"IdleR", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.5f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y * 0.75f), 1.0f / 10.f);
	AnimationClip* IdleU = new AnimationClip(L"IdleU", playerTex, 1, DirectX::XMFLOAT2(0, texSize.y * 0.75f), DirectX::XMFLOAT2(texSize.x * 0.25f, texSize.y), 1.0f / 10.0f);

	// Walk anim
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
	//SetCollision(20, 20, 20, 20);

	// weapon
	sword = new Weapon(WeaponType::SWORD, 0);

	// up/down stair effect
	fadeEffect = new ChangeMapEffect();

	SAFE_DELETE(playerTex);

	// InitPlayerStatus
	playerData.HP = 10000;
	playerData.atk = 10;
	playerData.def = 10;
	playerData.mdf = 0;
	playerData.exp = 0;
	playerData.gold = 0;
	playerData.level = 1;
	allowControl = true;
	player = this;
}

void Player::PlayFadeEffect(bool fade)
{
	if (fade)
	{
		fadeEffect->SetFading(true);
		fadeEffect->StartFadeOut();
	}
		
	else
		fadeEffect->StartFadeIn();
		
}

Player::~Player()
{
	SAFE_DELETE(armor);
	SAFE_DELETE(sword);
	SAFE_DELETE(fadeEffect);
	cleanUpCompletedCoroutines();
}

void Player::UpdateGameMode()
{
	if (!keyboard.Press('X'))
		mApp.SetAllowSwitch(true);
	if (keyboard.Press('X') && mApp.AvailableToSwitch())
	{
		mApp.SetAllowSwitch(false);
		// only change game mode to displaymenu if on game play mode
		if (mApp.GetGameMode() == GameMode::GAMEPLAY)
		{
			mApp.SetGameMode(GameMode::DISPLAYMENU);
		}
	}
}

void Player::UpdateAttackAnim()
{
	if (playAttackAnim)
	{
		DirectX::XMFLOAT3 attackPosition = *(animRect->GetPos());
		switch (animRect->GetCurDirection())
		{
		case PlayerControl::Up:
			attackPosition += DirectX::XMFLOAT3(0.0f, TileHeight, 0.0f);
			break;
		case PlayerControl::Down:
			attackPosition += DirectX::XMFLOAT3(0.0f, 0.0f - TileHeight, 0.0f);
			break;
		case PlayerControl::Left:
			attackPosition += DirectX::XMFLOAT3(0.0f - TileWidth, 0.0f, 0.0f);
			break;
		case PlayerControl::Right:
			attackPosition += DirectX::XMFLOAT3(TileWidth, 0.0f, 0.0f);
			break;
		}
		DirectX::XMFLOAT3 adjust = DirectX::XMFLOAT3(TileWidth / 2, TileHeight / 2, 0.0f);
		sword->UpdateAttackEffect(attackPosition - adjust);
	}
}

void Player::UpdateFadeEffect()
{
	if (fadeEffect->GetFading())
		fadeEffect->Update();
}

void Player::UpdateWalkRestricted()
{
	if (walkRestricted)
	{
		std::wstring str = Map::get_instance().GetEventNameByCoord(eventCoord);
		if (str.compare(0, 5, L"Arrow") != 0)
		{
			ResetWalkable();
		}
	}
}

void Player::UpdateShortCutButton()
{
	// telewarp
	if (items.find(ItemID::TELEWARP) != items.end() && keyboard.Up('G'))
		ButtonOnClick::floorWarp();
}

void Player::Update()
{
	UpdateGameMode();

	animator->Update();
	animRect->Update();

	UpdateAttackAnim();
	UpdateFadeEffect();
	UpdateWalkRestricted();
	UpdateShortCutButton();

	cleanUpCompletedCoroutines();
}

void Player::Render()
{
	if (renderPlayer)
		animRect->Render();
	if (playAttackAnim)
		sword->RenderAttackEffect();
	if (fadeEffect->GetFading())
		fadeEffect->Render();
}

void Player::AddItem(ItemID itemID)
{
	if (items.find(itemID) == items.end())
	{
		mApp.ShowMessageDialogue(static_cast<int>(itemID));
	}
		
	items[itemID]++;
}

bool Player::UseItem(ItemID itemID)
{
	if (items[itemID] > 0) 
	{ 
		items[itemID]--; 
		return true; 
	}
	else 
		return false;
}

VisitedFloorRange Player::GetVisitedFloorRange()
{
	int max = INT_MIN, min = INT_MAX;
	for (auto& floorNum : visitedFloor)
	{
		if (max < floorNum)
			max = floorNum;
		if (min > floorNum)
			min = floorNum;
	}
	return VisitedFloorRange(min, max);
}

bool Player::CanMove(const DirectX::XMFLOAT3& move)
{
	collisionBoxes = Map::get_instance().GetCollisionBoxes();
	unwalkableTiles = Map::get_instance().GetUnwalkableTiles();
	if (Keyboard::get_instance().Press(VK_CONTROL))
		return true;
	BoundingBox* predictedBox = new BoundingBox(*animRect->GetBox());
	predictedBox->SetEdge(
		predictedBox->GetEdge()->LT + move,
		predictedBox->GetEdge()->RB + move
		);
	for (const auto& unwalkableTile : *(unwalkableTiles)) 
	{
		if (unwalkableTile->AABB(predictedBox))
		{
			SAFE_DELETE(predictedBox);
			return false;
		}
	}
	for (const auto& collisionBox : *(collisionBoxes))
	{
		if (collisionBox->AABB(predictedBox))
		{
			if (!collisionBox->repeatCollisionEvent)
			{
				if (coroutinesNonRepeat.find(collisionBox->UUID) == coroutinesNonRepeat.end())
				{
					// hashmap to make sure one bounding box collison only happen once
					std::shared_ptr<Coroutine> coro = std::make_shared<Coroutine>();
					coroutinesNonRepeat[collisionBox->UUID] = coro;
					collisionBox->handleCollision(*coro);
				}
			}
			else if (!isCoroutineRunning)
			{
				isCoroutineRunning = true;
				std::shared_ptr<Coroutine> coro = std::make_shared<Coroutine>();
				coroutines.push_back(coro);
				collisionBox->handleCollision(*coro);
			}

			switch (collisionBox->colliderType)
			{
			case (ColliderType::BLOCKING):
			{
				SAFE_DELETE(predictedBox);
				return false;
			}
			case (ColliderType::TRIGGER):
			{
				SAFE_DELETE(predictedBox);
				return true;
			}
			}
		}
	}
	SAFE_DELETE(predictedBox);
	return true;
}

void Player::cleanUpCompletedCoroutines() 
{
	coroutines.erase(
		std::remove_if(coroutines.begin(), coroutines.end(),
			[this](const std::shared_ptr<Coroutine>& coro) {
				if (coro->isCompleted()) {
					isCoroutineRunning = false;  // Reset the flag when coroutine is completed
					return true;
				}
				return false;
			}),
		coroutines.end());

	std::lock_guard<std::mutex> lock(mtx);  // Ensure thread safety
	for (auto it = coroutinesNonRepeat.begin(); it != coroutinesNonRepeat.end();) {
		if (it->second->isCompleted()) {
			it = coroutinesNonRepeat.erase(it);  // Erase completed coroutine
		}
		else {
			++it;
		}
	}
}