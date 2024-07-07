#include "GeneralEvent.h"
#include "Player.h"
#include "MapStatic.h"
#include "Database.h"

void GeneralEvent::OnPlayerCollision(Coroutine& coro)
{
	if (triggerOnce)
	{
		// update event message to destroy the event
		std::shared_ptr<Message> eventUpdate = std::make_shared<MessageEventUpdate>(this->eventName);
		MessageDispatcher::get_instance().dispatch("DestroyEvents", eventUpdate);
		MapStatic::eventParams[Player::player->GetCurFloor()].erase(eventName);
		destroy = true;
	}

	Database::generalEventFuncMap[triggerID]();
	

	coro.setComplete();
};


GeneralEvent::GeneralEvent(Coord coord, UINT triggerID, ColliderType type, bool triggerOnce, std::wstring eventName, DirectX::XMFLOAT3 size)
	:GameEvent(coord, size, EventType::DEFAULT, eventName), triggerID(triggerID), colliderType(type), triggerOnce(triggerOnce)
{
	SetupAnimRect(colliderType);

	// lamda expression  to capture current object and bind its member functions as callbacks.
	animRect->SetOnCollision([this](Coroutine& coro) { this->OnPlayerCollision(coro); });
}

GeneralEvent::~GeneralEvent()
{

}

void GeneralEvent::Update()
{

}

void GeneralEvent::Render()
{

}
