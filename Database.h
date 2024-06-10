#pragma once

#include "Util.h"
#include "GameEvent.h"

struct MonsterData
{
	MonsterData(std::wstring name, UINT id, UINT HP, UINT attack, UINT defense, UINT gold, UINT exp)
		:name(name), id(id), HP(HP), attack(attack), defense(defense), gold(gold), exp(exp) {}
	MonsterData() = default;
	std::wstring name;
	UINT id;
	UINT HP;
	UINT attack;
	UINT defense;
	UINT gold;
	UINT exp;
};

class Database
{
public:
	// key: monsterID	value: monster status
	static std::unordered_map<UINT, MonsterData> monsterDatabase;
};