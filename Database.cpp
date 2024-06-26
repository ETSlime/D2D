#include "Database.h"

std::wstring Database::branch = DIALOGUE_BRANCH;

std::unordered_map<ItemID, std::tuple<std::wstring, std::wstring>> Database::itemCategoryMap = 
{
	{ ItemID::YELLOW_KEY, std::make_tuple(L"鍵類", L"黄色の鍵")},
	{ ItemID::BLUE_KEY,  std::make_tuple(L"鍵類", L"紫の鍵")},
	{ ItemID::RED_KEY,  std::make_tuple(L"鍵類", L"赤い鍵")},
	{ ItemID::BOOK,  std::make_tuple(L"工具類",L"本")},
	{ ItemID::BOOMB,  std::make_tuple(L"重要類",L"爆弾")}
};

std::unordered_map<UINT, std::wstring> Database::itemGetText =
{
	{static_cast<UINT>(ItemID::YELLOW_KEY), L""},
	{static_cast<UINT>(ItemID::BLUE_KEY), L""},
	{static_cast<UINT>(ItemID::RED_KEY), L""},
	{static_cast<UINT>(ItemID::BOOK),  L"あなたは「モンスターマニュアル」を手に入れました\n\n所持するとモンスター情報を確認できます "},
};

std::unordered_map<UINT, std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>> Database::dialogues =
{
	{0, 
		std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>(
			{std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n1", L"a"),
			std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n1", L"b")}),
	},
	{1,
		std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>(
			{std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n1", L"c"),
			std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n2", L"d")}),
	},
	{2,
		std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>(
			{std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n1", L"あああああああああああああああああああああああああああああああああああああああああああああああああああああああ"),
			std::make_tuple(std::vector<DialogueButtonEvent>{DialogueButtonEvent::ADDHP, DialogueButtonEvent::ADDMP},
				L"n1", L"ああああああああ: \n" + branch + L"あああああああ:\nああああ:"),
			std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n2", L"d")
			}),
	},
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