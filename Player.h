#pragma once

#include "GameEvent.h"
#include "AnimationRect.h"
#include "Weapon.h"
#include <mutex>

class Player : public GameEvent
{
public:

	Player(Coord coord, std::wstring PlayerTexture, DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }

	Weapon* armor = nullptr;
	Weapon* sword = nullptr;

	bool CanMove(const DirectX::XMFLOAT3& move);

	static Player* player;

	bool playAttackAnim = false;

private:

	const std::vector<BoundingBox*>* unwalkableTiles = Map::get_instance().GetUnwalkableTiles();
	const std::vector<BoundingBox*>* collisionBoxes = Map::get_instance().GetCollisionBoxes();

	// Store active coroutines
	std::vector<std::shared_ptr<Coroutine>> coroutines;  
	// Track if a coroutine is already running
	std::atomic<bool> isCoroutineRunning;  
	// Store active coroutines correspoindign to boundingbox
	std::unordered_map<std::string, std::shared_ptr<Coroutine>> coroutinesNonRepeat;  
	std::mutex mtx;  // Mutex to protect coroutines map
	void cleanUpCompletedCoroutines();
};

