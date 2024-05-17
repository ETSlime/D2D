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


	//class Collisionengine* engine = nullptr;

	int play = 0;
	float time = 0.0f;
	float timechecker = 0.0f;
	float arrowtime = 2.0f;
	int PauseCount = 0;

	bool Valid = true;

	int PoolCount = 0;
	int deathCount = 0;

	bool valid = true;
	bool destroy = false;

};