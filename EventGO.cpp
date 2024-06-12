#include "EventGO.h"
#include "String.h"

EventGO::EventGO(std::shared_ptr<GameEvent>(event)) 
{ 
	this->event = event;
}

void EventGO::Init()
{

}

void EventGO::Destroy()
{
	this->event.reset();
}

void EventGO::Update()
{
	event->Update();
	if (event->GetDestroyed())
	{
		SetIsDestroyed(true);
	}
		
}

void EventGO::Render()
{
	event->Render();
}

void EventGO::PostRender()
{
	event->PostRender();
}

void EventGO::GUI()
{
}

bool EventGO::IsValid()
{
	return valid;
}

bool EventGO::IsDestroyed()
{
	return destroy;
}

void EventGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void EventGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}