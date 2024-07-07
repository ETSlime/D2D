#pragma once

#include "Util.h"


#define DIALOGUE_BRANCH				(L"branch:\n")

/*
* name
* id
* HP
* attack
* defense
* gold
* exp
*/

enum class ItemID : UINT
{
	YELLOW_KEY,
	BLUE_KEY,
	RED_KEY,
	GREEN_KEY,
	GRAY_KEY,
	IRON_KEY,
	SPECIAL_YELLOW_KEY,
	WHITE_KEY,
	YELLOW_KEY_2,
	BLUE_KEY_2,
	RED_KEY_2,
	GREEN_KEY_2,
	BOOK,
	NOTE,
	TELEWARP,
	LUCKY_COIN,
	HOE,
	HOE_2,
	ICE,
	EARTHQUAKE_SCROLL,
	SYMMETRIC_FLYER,
	DOWNSTAIR_FLYER,
	UPSTAIR_FLYER,
	CROSS,
	BOOMB,
	LARGE_BOOMB,
	PLACEHOLDER2,
	KINDLE,
	SHOES,
	BAGS,
	PLACEHOLDER3,
	PLACEHOLDER4,
	ATT_GEM,
	DEF_GEM,
	MATK_GEM,
	MDEF_GEM,
	RED_POTION,
	BLUE_POTION,
	GREEN_POTION,
	YELLOW_POTION,
	GREEN_POT,
	PINK_POT,
	BLUE_POT,
	PLACEHOLDER5,
	PLACEHOLDER6,
	PLACEHOLDER7,
	ANTIDOTE,
	UNIVERSAL_ANTIDOTE
};


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

enum class DialogueButtonEvent
{
	NONE,
	ADDHP,
	ADDMP
};

class Database
{
public:
	// key: monsterID	value: monster status
	static std::unordered_map<UINT, MonsterData> monsterDatabase;

	// Map item IDs to their categories
	// key: itemID		value: item category name, item name
	static std::unordered_map<ItemID, std::tuple<std::wstring, std::wstring>> itemCategoryMap;

	//  item get
	static std::unordered_map<UINT, std::wstring> itemGetText;

	// npc dialogue
	// key: dialogue ID		value: tuple of speakers' name and dialogue context
	static std::unordered_map<UINT, std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>> dialogues;
	static std::wstring branch;

	// item button function map
	static std::unordered_map<ItemID, std::function<void()>> buttonFuncMap;

	// general event function pointer
	static std::unordered_map<UINT, std::function<void()>> generalEventFuncMap;
};