#include "Door.h"
#include "MessageDispatcher.h"
#include "Coroutine.h"

constexpr UINT OPENDOOR_ANIM_FRAME = 4;
constexpr float ANIM_PLAY_SPEED = 15.0f;

void Door::OnPlayerCollision(Coroutine& coro)
{
	std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
	animator->SetCurrentAnimClip(L"Open");
	if (coro.getState() == 0) 
	{	
		coro.yield(OPENDOOR_ANIM_FRAME / ANIM_PLAY_SPEED);
	}
	if (coro.getState() == 1) 
	{
		MessageDispatcher::get_instance().dispatch("UpdateEvents", eventUpdate);
		destroy = true;
		// final state, complete the coroutine, otherwise will cause memory leak
		coro.setComplete();
	}
};

Door::Door(Coord coord, DoorType type, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::DOOR, eventName), doorType(type)
{
	std::wstring DoorTexture;
	DoorTexture = L"Door";
	if (static_cast<UINT>(doorType) / OPENDOOR_ANIM_FRAME + 1 < 10)
		DoorTexture += L"00";
	else
		DoorTexture += L"0";
	DoorTexture += std::to_wstring(1 + static_cast<UINT>(doorType) / OPENDOOR_ANIM_FRAME);
	DoorTexture += L".png";
	Texture2D* DoorTex = new Texture2D(GameEventsPath + DoorTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(DoorTex->GetWidth(), DoorTex->GetHeight());

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", DoorTex, 1, DirectX::XMFLOAT2(static_cast<UINT>(doorType) % OPENDOOR_ANIM_FRAME * 0.25f * texSize.x, 0.0f),
		DirectX::XMFLOAT2((static_cast<UINT>(doorType) % OPENDOOR_ANIM_FRAME + 1) * 0.25f * texSize.x, 0.25f * texSize.y), 1.0f / ANIM_PLAY_SPEED);

	// open door Anim
	AnimationClip* open = new AnimationClip(L"Open", DoorTex, OPENDOOR_ANIM_FRAME, DirectX::XMFLOAT2(static_cast<UINT>(doorType) % OPENDOOR_ANIM_FRAME * 0.25f * texSize.x, 0.0f),
		DirectX::XMFLOAT2((static_cast<UINT>(doorType) % OPENDOOR_ANIM_FRAME + 1) * 0.25f * texSize.x, texSize.y), 1.0f / ANIM_PLAY_SPEED, true);

	//	clip save
	animator->SetAnim(Idle);
	animator->SetAnim(open);
	animator->SetCurrentAnimClip(L"Idle");


	SetupAnimRect(ColliderType::BLOCKING);
	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });

	SAFE_DELETE(DoorTex);
}

Door::~Door()
{


}

void Door::Update()
{
	animator->Update();
	animRect->Update();

}

void Door::Render()
{
	animRect->Render();
}

