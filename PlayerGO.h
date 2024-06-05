#pragma once
#define PoolSize 15
#define ClearTime 60
#define EnemySelection 4

#include "IGameObj.h"
#include "Util.h"
#include "Player.h"

class PlayerGO : public IGameObj
{
public:

	PlayerGO(Coord coord, std::wstring name = L"Default") : playerCoord(coord), name(name) {};

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
	Player* player = nullptr;

	bool valid = true;
	bool destroy = false;

	Coord playerCoord;
	std::wstring name;

};