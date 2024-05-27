#include "Door.h"
#include "MessageDispatcher.h"

void Door::OnPlayerCollision() 
{
	std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
	MessageDispatcher::get_instance().dispatch("UpdateEvents", eventUpdate);
	animator->SetCurrentAnimClip(L"Open");
	destroy = true;
};

Door::Door(Coord coord, DoorType type, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::DOOR, eventName), doorType(type)
{
	std::wstring DoorTexture;
	DoorTexture = L"Door";
	if (static_cast<UINT>(doorType) / 4 + 1 < 10)
		DoorTexture += L"00";
	else
		DoorTexture += L"0";
	DoorTexture += std::to_wstring(1 + static_cast<UINT>(doorType) / 4);
	DoorTexture += L".png";
	Texture2D* DoorTex = new Texture2D(GameEventsPath + DoorTexture);
	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(DoorTex->GetWidth(), DoorTex->GetHeight());

	// idle Anim
	AnimationClip* Idle = new AnimationClip(L"Idle", DoorTex, 1, DirectX::XMFLOAT2(static_cast<UINT>(doorType) % 4 * 0.25f * texSize.x, 0.0f),
		DirectX::XMFLOAT2((static_cast<UINT>(doorType) % 4 + 1) * 0.25f * texSize.x, 0.25f * texSize.y), 1.0f / 10.0f);

	// open Anim
	AnimationClip* open = new AnimationClip(L"Open", DoorTex, 4, DirectX::XMFLOAT2(static_cast<UINT>(doorType) % 4 * 0.25f * texSize.x, 0.0f),
		DirectX::XMFLOAT2((static_cast<UINT>(doorType) % 4 + 1) * 0.25f * texSize.x, texSize.y), 1.0f / 10.0f, true);

	//	clip save
	animator->SetAnim(Idle);
	animator->SetAnim(open);
	animator->SetCurrentAnimClip(L"Idle");
	animRect->SetAnimation(animator);

	// bounding box
	animRect->UpdateBoundingBox();
	animRect->SetBoundingBoxType(ColliderType::BLOCKING);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this] { this->OnPlayerCollision(); });

	animRect->SetEvent(this);
	
	////collision edge
	//float LT_x = map.GetPositionFromCoord(coord).x;
	//float LT_y = map.GetPositionFromCoord(coord).y + TileHeight;
	//float RB_x = map.GetPositionFromCoord(coord).x + TileWidth;
	//float RB_y = map.GetPositionFromCoord(coord).y;
	//SetCollision(LT_x, LT_y, RB_x, RB_y);


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

