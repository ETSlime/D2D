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
	if (this->event)
		this->event.reset();
}

void EventGO::Update()
{
	if (event)
	{
		event->Update();
		if (event->GetDestroyed())
		{
			SetIsDestroyed(true);
		}
	}

		
}

void EventGO::Render()
{
	if (event)
		event->Render();
}

void EventGO::PostRender()
{
	if (event)
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