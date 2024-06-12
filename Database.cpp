#include "Database.h"

std::unordered_map<ItemID, std::tuple<std::wstring, ItemCategory>> Database::itemCategoryMap = 
{
	{ ItemID::YELLOW_KEY, std::make_tuple(L"����", ItemCategory::KEYS)},
	{ ItemID::BLUE_KEY,  std::make_tuple(L"����", ItemCategory::KEYS) },
	{ ItemID::RED_KEY,  std::make_tuple(L"����", ItemCategory::KEYS) },
	{ ItemID::BOOK,  std::make_tuple(L"�H���",ItemCategory::AUXILIARIES) },
	{ ItemID::BOOMB,  std::make_tuple(L"�d�v��",ItemCategory::TREASURES) }
};

std::unordered_map<UINT, MonsterData> Database::monsterDatabase =
{
	{0,
		MonsterData(L"G�X���C��", 0, 35, 18, 1, 1, 0)
	},
	{1,
		MonsterData(L"R�X���C��", 1, 45, 20, 2, 2, 0)
	},
	{2,
		MonsterData(L"B�X���C��", 2, 130, 60, 3, 8, 0)
	},
	{3,
		MonsterData(L"�X���C���L���O", 3, 360, 310, 20, 40, 0)
	},
	{4,
		MonsterData(L"�o�b�g", 4, 35, 38, 3, 3, 0)
	},
	{5,
		MonsterData(L"��o�b�g", 5, 60, 100, 8, 12, 0)
	},
	{6,
		MonsterData(L"R�o�b�g", 6, 130, 60, 3, 8, 0)
	},
	{7,
		MonsterData(L"�X�P���g��", 7, 36000, 310, 20, 40, 0)
	},
};