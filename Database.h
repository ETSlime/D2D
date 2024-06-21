#pragma once

#include "Util.h"
#include "GameEvent.h"
#include "Item.h"

/*
* name
* id
* HP
* attack
* defense
* gold
* exp
*/
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

	// Map item IDs to their categories
	// key : itemID		value: item category name, item name
	static std::unordered_map<ItemID, std::tuple<std::wstring, std::wstring>> itemCategoryMap;

	//  item get
	static std::unordered_map<UINT, std::wstring> itemGetText;

	// npc dialogue
	static std::unordered_map<UINT, std::vector<std::tuple<std::wstring, std::wstring>>> dialogues;
};