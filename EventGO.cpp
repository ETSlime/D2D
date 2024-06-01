#include "EventGO.h"


EventGO::EventGO(std::shared_ptr<GameEvent>(event)) 
{ 
	this->event = event;
}

void EventGO::Init()
{

}

void EventGO::Destroy()
{

}

void EventGO::Update()
{
	event->Update();
	if (event->destroy)
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