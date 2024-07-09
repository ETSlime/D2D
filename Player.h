#pragma once

#include "GameEvent.h"
#include "Weapon.h"
#include <mutex>

class AnimationRect;
class ChangeMapEffect;

struct PlayerData
{
	int HP, MP, atk, def, mdf, exp, gold, level;
};

struct VisitedFloorRange
{
	int Vmin;
	int Vmax;

	VisitedFloorRange(int Vmin, int Vmax) :Vmin(Vmin), Vmax(Vmax) {}
};

class Player : public GameEvent
{
public:

	friend class ButtonOnClick;

	Player(Coord coord, std::wstring PlayerTexture, DirectX::XMFLOAT3 size = { TileWidth, TileHeight, 1 });
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	AnimationRect* GetanimRect() { return animRect; }



	bool CanMove(const DirectX::XMFLOAT3& move);
	void PlayFadeEffect(bool fade);
	void SetAllowControl(bool allow) { allowControl = allow; }
	bool GetAllowControl() { return allowControl; }
	void SetFacingDirection(PlayerControl::Direction dir) { animRect->SetFacingWhere(dir); }
	bool GetDirectionWalkable(PlayerControl::Direction dir) { return walkable[dir]; }
	// left/right/up/down
	void SetDirectionWalkable(std::array<bool, 4> buffer) { walkableBuffer = buffer; walkRestricted = true; }
	void UpdateWalkable() { walkable = walkableBuffer; }
	void ResetWalkable() { walkable = { true, true, true, true }; walkRestricted = false; }
	bool GetWalkRestricted() { return walkRestricted; }
	static Player* player;
	
	bool playAttackAnim = false;
	
	bool renderPlayer = false;
	ChangeMapEffect* fadeEffect;

	int GetCurFloor() { return curFloor; }
	UINT GetWalkingSteps(){ return walkingSteps; }
	void StepsCountOne() { walkingSteps++; }
	void SetCurFLoor(int newFloorNum) { curFloor = newFloorNum; }
	const std::map<ItemID, UINT>& GetItems() const { return items; }
	const std::set<int>& GetVisitedFloor() const { return visitedFloor; }

	void AddItem(ItemID itemID);
	bool UseItem(ItemID itemID);
	void VisitFloor(int curFloor) { visitedFloor.insert(curFloor); }
	void ChangeHP(int amount) { playerData.HP += amount; if (playerData.HP < 0) playerData.HP = 0; }
	void ChangeAtk(int amount) { playerData.atk += amount; }
	void ChangeDef(int amount) { playerData.def += amount; }
	void ChangeMdf(int amount) { playerData.mdf += amount; }
	void ChangeExp(int amount) { playerData.exp += amount; }
	void ChangeGold(int amount) { playerData.gold += amount; }
	const PlayerData GetBattleData() { return playerData; }

	VisitedFloorRange GetVisitedFloorRange();
	
private:

	// updates
	void UpdateGameMode();
	void UpdateAttackAnim();
	void UpdateFadeEffect();
	void UpdateWalkRestricted();
	void UpdateShortCutButton();

	// player attributes
	PlayerData playerData;
	UINT walkingSteps = 0;
	std::map<ItemID, UINT> items;
	std::set<int> visitedFloor;
	int curFloor = 0;
	bool allowControl = true;
	bool walkRestricted = false;
	// left/right/up/down
	std::array<bool, 4> walkable = { true, true, true, true };
	std::array<bool, 4> walkableBuffer = { true, true, true, true };
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

