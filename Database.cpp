#include "Database.h"

std::unordered_map<ItemID, std::tuple<std::wstring, ItemCategory>> Database::itemCategoryMap = 
{
	{ ItemID::YELLOW_KEY, std::make_tuple(L"鍵類", ItemCategory::KEYS)},
	{ ItemID::BLUE_KEY,  std::make_tuple(L"鍵類", ItemCategory::KEYS) },
	{ ItemID::RED_KEY,  std::make_tuple(L"鍵類", ItemCategory::KEYS) },
	{ ItemID::BOOK,  std::make_tuple(L"工具類",ItemCategory::AUXILIARIES) },
	{ ItemID::BOOMB,  std::make_tuple(L"重要類",ItemCategory::TREASURES) }
};

std::unordered_map<UINT, MonsterData> Database::monsterDatabase =
{
	{0,
		MonsterData(L"Gスライム", 0, 35, 18, 1, 1, 0)
	},
	{1,
		MonsterData(L"Rスライム", 1, 45, 20, 2, 2, 0)
	},
	{2,
		MonsterData(L"Bスライム", 2, 130, 60, 3, 8, 0)
	},
	{3,
		MonsterData(L"スライムキング", 3, 360, 310, 20, 40, 0)
	},
	{4,
		MonsterData(L"バット", 4, 35, 38, 3, 3, 0)
	},
	{5,
		MonsterData(L"大バット", 5, 60, 100, 8, 12, 0)
	},
	{6,
		MonsterData(L"Rバット", 6, 130, 60, 3, 8, 0)
	},
	{7,
		MonsterData(L"スケルトン", 7, 36000, 310, 20, 40, 0)
	},
};