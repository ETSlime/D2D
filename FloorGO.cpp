#include "FloorGO.h"
#include "EventGO.h"
#include "Stair.h"

FloorGO::FloorGO(int floor):floor(floor){}

void FloorGO::Init()
{
	Player::player->SetCurFLoor(floor);
	Player::player->VisitFloor(floor);
	tileMap = new TMap(floor);
	map.GenerateEvent(floor);
	UINT eventGOID = 0;
	for (auto& event : map.curEvents)
	{
		std::wstring EventGOName;

		if (event.second.get() && event.second.get()->GetEventType() == EventType::STAIR)
			dynamic_cast<Stair*>(event.second.get())->SetCurrentFloor(floor);

		EventGOName = L"EventGO" + std::to_wstring(eventGOID++);
		
		mApp.Push(EventGOName, std::make_unique<EventGO>(event.second));
		curEventsGOName.push_back(EventGOName);
	}
}

void FloorGO::Destroy()
{
	SAFE_DELETE(tileMap);
	map.curEvents.clear();
	for (auto& name : curEventsGOName)
	{
		mApp.DestroyGO(name);
	}
}

void FloorGO::Update()
{
	tileMap->Update();
}

void FloorGO::Render()
{
	tileMap->Render();
}

void FloorGO::PostRender()
{
}

void FloorGO::GUI()
{
}

bool FloorGO::IsValid()
{
	return valid;
}

bool FloorGO::IsDestroyed()
{
	return destroy;
}

void FloorGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void FloorGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}