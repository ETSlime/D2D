#include "Item.h"
#include "Player.h"
#include "MapStatic.h"

constexpr UINT IDLE_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 10.0f;

void Item::OnPlayerCollision(Coroutine& coro)
{
	// update event message to destroy the event
	std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
	MessageDispatcher::get_instance().dispatch("DestroyEvents", eventUpdate);

	// check if item effects directly
	if (Database::directItemEffect.find(itemID) != Database::directItemEffect.end())
		Database::directItemEffect[itemID]();
	else // otherwise, add to item list
		Player::player->AddItem(itemID);

	destroy = true;
	MapStatic::eventParams[Player::player->GetCurFloor()].erase(eventName);
	coro.setComplete();
};

Item::Item(Coord coord, ItemID ID, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::ITEM, eventName), itemID(ID)
{
	std::wstring itemTexture;
	itemTexture = L"item";
	if (static_cast<UINT>(itemID) / 16 + 1 < 10)
		itemTexture += L"0";
	itemTexture += std::to_wstring(1 + static_cast<UINT>(itemID) / 16);
	itemTexture += L".png";
	Texture2D* itemTex = new Texture2D(GameEventsPath + itemTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(itemTex->GetWidth(), itemTex->GetHeight());
	float texStartX = static_cast<UINT>(itemID) % 4 * 0.25f * texSize.x;
	float texStartY = static_cast<UINT>(itemID) / 4 % 4 * 0.25f * texSize.y;
	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", itemTex, 1,
		DirectX::XMFLOAT2(texStartX, texStartY), DirectX::XMFLOAT2(texStartX + 0.25f * texSize.x, texStartY + 0.25f * texSize.y), 1.0f / 1.0f);

	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::TRIGGER);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(itemTex);
}

Item::~Item()
{


}

void Item::Update()
{
	animRect->Update();
}

void Item::Render()
{
	animRect->Render();
}

