#include "Database.h"

std::wstring Database::branch = DIALOGUE_BRANCH;

std::unordered_map<ItemID, std::tuple<std::wstring, std::wstring>> Database::itemCategoryMap = 
{
	{ ItemID::YELLOW_KEY, std::make_tuple(L"����", L"���F�̌�")},
	{ ItemID::BLUE_KEY,  std::make_tuple(L"����", L"���̌�")},
	{ ItemID::RED_KEY,  std::make_tuple(L"����", L"�Ԃ���")},
	{ ItemID::BOOK,  std::make_tuple(L"�H���",L"�{")},
	{ ItemID::BOOMB,  std::make_tuple(L"�d�v��",L"���e")}
};

std::unordered_map<UINT, std::wstring> Database::itemGetText =
{
	{static_cast<UINT>(ItemID::YELLOW_KEY), L""},
	{static_cast<UINT>(ItemID::BLUE_KEY), L""},
	{static_cast<UINT>(ItemID::RED_KEY), L""},
	{static_cast<UINT>(ItemID::BOOK),  L"���Ȃ��́u�����X�^�[�}�j���A���v����ɓ���܂���\n\n��������ƃ����X�^�[�����m�F�ł��܂� "},
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
				L"n1", L"��������������������������������������������������������������������������������������������������������������"),
			std::make_tuple(std::vector<DialogueButtonEvent>{DialogueButtonEvent::ADDHP, DialogueButtonEvent::ADDMP},
				L"n1", L"����������������: \n" + branch + L"��������������:\n��������:"),
			std::make_tuple(std::vector<DialogueButtonEvent>(),
				L"n2", L"d")
			}),
	},
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