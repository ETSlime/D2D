#pragma once

#include "IGameObj.h"
#include "MagicTowerApp.h"
#include "TMap.h"
#include "Util.h"

class FloorGO : public IGameObj
{
public:
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

	FloorGO(int floor);

private:

	class TMap* tileMap = nullptr;

	bool valid = true;
	bool destroy = false;

	int floor;

	Map& map = Map::get_instance();
	MagicTowerApp& mApp = MagicTowerApp::get_instance();
};