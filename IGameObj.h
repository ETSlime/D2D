#pragma once

class IGameObj
{
public:
	virtual void Init() = 0;
	virtual void Destroy() = 0;	

	virtual void Update() = 0;

	virtual void Render() = 0;
	virtual void PostRender() = 0;	
	virtual bool IsValid() = 0;	
	virtual bool IsDestroyed() = 0;
	virtual void SetIsValid(bool _valid) = 0;
	virtual void SetIsDestroyed(bool _destroy) = 0;

	virtual void GUI() = 0;

};