#include "Monster.h"
#include "MapStatic.h"
#include "Battle.h"

constexpr UINT IDLE_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 10.0f;

void Monster::OnPlayerCollision(Coroutine& coro)
{

	std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
	
	if (coro.getState() == 0)
	{
		int damage = Battle::CalculateDamage(monsterData);
		if (damage == -1 || damage > Player::player->GetBattleData().HP)
		{
			coro.setComplete();
		}
		else
		{
			Player::player->playAttackAnim = true;
			Player::player->ChangeHP(-1 * damage);
			coro.yield(0.2f);
		}

	}
	if (coro.getState() == 1)
	{
		
		MessageDispatcher::get_instance().dispatch("DestroyEvents", eventUpdate);
		destroy = true;
		MapStatic::eventParams[Player::player->GetCurFloor()].erase(eventName);
		Player::player->playAttackAnim = false;
		// final state, complete the coroutine, otherwise will cause memory leak
		coro.setComplete();
	}
};

Monster::Monster(Coord coord, UINT monsterID, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::MONSTER, eventName), monsterID(monsterID)
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
	AnimationClip* Idle = new AnimationClip(L"Idle", MonsterTex, IDLE_ANIM_FRAME, DirectX::XMFLOAT2(0, monsterID % 4 * 0.25f * texSize.y), 
		DirectX::XMFLOAT2(texSize.x, (monsterID % 4 + 1) * 0.25f * texSize.y), 1.0f / ANIM_PLAY_SPEED);

	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::BLOCKING);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(MonsterTex);
	
	monsterData = Database::monsterDatabase[monsterID];
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

