#pragma once

#include "GameEvent.h"
#include "Weapon.h"
#include <mutex>

class AnimationRect;
class ChangeMapEffect;

class Player : public GameEvent
{
public:

	Player(Coord coord, std::wstring PlayerTexture, DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }



	bool CanMove(const DirectX::XMFLOAT3& move);
	void PlayFadeEffect(bool fade);

	static Player* player;
	
	bool playAttackAnim = false;
	bool allowControl = true;
	bool renderPlayer = false;
	ChangeMapEffect* fadeEffect;

	int GetCurFloor() { return curFloor; }
	void SetCurFLoor(int newFloorNum) { curFloor = newFloorNum; }

	void AddItem(ItemID itemID) { items[itemID]++; }
	bool UseItem(ItemID itemID) { if (items[itemID] > 0) { items[itemID]--; return true; } else return false; }
	void ChangeHP(UINT amout) { HP += amout; }

private:

	// player attributes
	int HP, MP, atk, def;
	std::unordered_map<ItemID, UINT> items;
	int curFloor = 0;

	// player weapons
	Weapon* armor = nullptr;
	Weapon* sword = nullptr;

	Keyboard& keyboard = Keyboard::get_instance();
	MagicTowerApp& mApp = MagicTowerApp::get_instance();

	// get every collison box in this map
	const std::vector<BoundingBox*>* unwalkableTiles = Map::get_instance().GetUnwalkableTiles();
	const std::vector<BoundingBox*>* collisionBoxes = Map::get_instance().GetCollisionBoxes();

	// Store active coroutines
	std::vector<std::shared_ptr<Coroutine>> coroutines;  
	// Track if a coroutine is already running
	std::atomic<bool> isCoroutineRunning = false;  
	// Store active coroutines correspoindign to boundingbox
	std::unordered_map<std::string, std::shared_ptr<Coroutine>> coroutinesNonRepeat;  
	std::mutex mtx;  // Mutex to protect coroutines map
	void cleanUpCompletedCoroutines();
};

