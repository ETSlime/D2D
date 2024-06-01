#include "Item.h"

constexpr UINT IDLE_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 10.0f;

void Item::OnPlayerCollision(Coroutine& coro)
{
	std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
	MessageDispatcher::get_instance().dispatch("UpdateEvents", eventUpdate);
	destroy = true;

	//if (coro.getState() == 0)
	//{
	//	Player::player->playAttackAnim = true;

	//	coro.yield(2);
	//}
	//if (coro.getState() == 1)
	//{

	//	MessageDispatcher::get_instance().dispatch("UpdateEvents", eventUpdate);
	//	destroy = true;
	//	Player::player->playAttackAnim = false;
	//	// final state, complete the coroutine, otherwise will cause memory leak
	//	coro.setComplete();
	//}
};

Item::Item(Coord coord, UINT ItemID, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::ITEM, eventName), itemID(ItemID)
{
	std::wstring ItemTexture;
	ItemTexture = L"item";
	if (ItemID / 4 + 1 < 10)
		ItemTexture += L"0";
	ItemTexture += std::to_wstring(1 + ItemID / 4);
	ItemTexture += L".png";
	Texture2D* ItemTex = new Texture2D(GameEventsPath + ItemTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(ItemTex->GetWidth(), ItemTex->GetHeight());
	float texStartX = ItemID % 4 * texSize.x;
	float texStartY = ItemID / 4 % 4 * 0.25f * texSize.y;
	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", ItemTex, 1, 
		DirectX::XMFLOAT2(texStartX, texStartY), DirectX::XMFLOAT2(texStartX + 0.25f * texSize.x, texStartY + 0.25f * texSize.y), 1.0f / 1.0f);

	//clip save
	animator->SetAnim(Idle);
	animator->SetCurrentAnimClip(L"Idle");

	SetupAnimRect(ColliderType::TRIGGER);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(ItemTex);
}

Item::~Item()
{


}

void Item::Update()
{
	animator->Update();
	animRect->Update();
}

void Item::Render()
{
	animRect->Render();
}

