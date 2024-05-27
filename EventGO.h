#pragma once

#include "Util.h"
#include "IGameObj.h"
#include "GameEvent.h"

class EventGO : public IGameObj
{
public:

	EventGO(std::shared_ptr<GameEvent>(event));

	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	virtual bool IsValid() override;
	virtual bool IsDestroyed() override;
	virtual void SetIsDestroyed(bool _destroy) override;
	virtual void SetIsValid(bool _valid) override;


private:

	std::shared_ptr<GameEvent> event;
	bool valid = true;
	bool destroy = false;
};