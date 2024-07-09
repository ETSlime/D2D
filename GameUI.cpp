#include "GameUI.h"
#include "Monster.h"
#include "Battle.h"
#include "SaveData.h"

GameUI::GameUI(const D2DResource* D2DResource, const WinSize* winSize, UIRenderMode mode, bool allowChange)
	:mD2DResource(D2DResource), curWinSize(winSize), renderMode(mode), allowChange(allowChange)
{
	InitGameUI(renderMode);
}

GameUI::~GameUI()
{
	SAFE_DELETE(base);
	SAFE_DELETE(rightDispalyBase);
	SAFE_DELETE(leftButtonBase);
	SAFE_DELETE(timerBase);
	SAFE_DELETE(walkingStepBase);
	SAFE_DELETE(itemCheckTitleBase);
	SAFE_DELETE(itemCheckDetailBase);
	SAFE_DELETE(itemCheckCategoryBase);
	SAFE_DELETE(itemCheckDescriptionBase);
	SAFE_DELETE(playerIcon);
	SAFE_DELETE(yellowKeyIcon);
	SAFE_DELETE(blueKeyIcon);
	SAFE_DELETE(redKeyIcon);
	SAFE_DELETE(greenKeyIcon);
	for (auto saveSlotBase : saveSlotBases)
		SAFE_DELETE(saveSlotBase);
	saveSlotBases.clear();
	SAFE_DELETE(dialogueBase);

	//monsters.clear();
	//startButtons.clear();
	//gameMenuButtons.clear();
	//itemCategoryButtons.clear();
	//playerIcons.clear();
}

void GameUI::InitGameUI(GameUI::UIRenderMode mode)
{
	renderMode = mode;

	// Initialize UI components based on render mode
	switch (renderMode)
	{
	case STARTMENU:
		this->InitMainMenu();
		break;
	case INGAMEUI:
		this->InitInGameUI();
		break;
	case PLAYERSTATES:
		this->InitPlayerState();
		break;
	case ITEMCHECK:
		this->InitItemCheck();
		break;
	case SAVEDATA:
		this->InitSaveLoadData();
		break;
	case LOADDATA:
		this->InitSaveLoadData();
		break;
	case DIALOGUE:
		this->InitDialogue();
		break;
	case MESSAGE:
		this->InitMessage();
		break;
	case WARP:
		this->InitWarp();
		break;
	default:
		break;
	}
}

void GameUI::InitMainMenu()
{
	startCursor.baseX = 0.50f;
	startCursor.baseY = 0.435f;
	startCursor.position = DirectX::XMFLOAT3(startCursor.baseX * curWinSize->width, startCursor.baseY * curWinSize->height, 0.0f);
	startCursor.size = DirectX::XMFLOAT3(300, 50, 1.0f);
	startCursor.curIdx = 0;
	if (startCursor.textureRect == nullptr)
		startCursor.textureRect = new CursorTextureRect(startCursor.position,
			startCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	startButtons.clear();
	startButtons.push_back(std::make_unique<Button>(L"始めから", DirectX::XMFLOAT3(0.4f, 0.535f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startNewGame, true));
	startButtons.push_back(std::make_unique<Button>(L"続きから", DirectX::XMFLOAT3(0.4f, 0.635f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::loadGameUI));
	startButtons.push_back(std::make_unique<Button>(L"ゲーム終了", DirectX::XMFLOAT3(0.4f, 0.735f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	startButtons.push_back(std::make_unique<Button>(L"遊ぶ方法説明", DirectX::XMFLOAT3(0.4f, 0.835f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));

	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(490, 220, 0);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(400, 320, 1);
	if (base == nullptr)
		base = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
}

void GameUI::InitInGameUI()
{
	monsters.clear();
	UINT index = 0;
	for (auto& event : map.curEvents)
	{	
		if (event.second.get()->GetEventType() == EventType::MONSTER
			&& monsters.find(std::static_pointer_cast<Monster>(event.second).get()->monsterData.id) == monsters.end())
		{
			MonsterData monsterData = std::static_pointer_cast<Monster>(event.second).get()->monsterData;
			DirectX::XMFLOAT3 monsterIonPosition = DirectX::XMFLOAT3(0, 0, 0);
			DirectX::XMFLOAT3 mosterIonSize = DirectX::XMFLOAT3(TileWidth * 0.8f, TileHeight * 0.8f, 1);
			Texture2D* monsterIconTex = new Texture2D(BattlersPath + monsterData.name + L".png");
			std::unique_ptr<TextureRect> monsterIcon = std::make_unique<TextureRect>(monsterIonPosition, mosterIonSize, 0.0f);
			monsterIcon->SetSRV(monsterIconTex->GetSRV());
			monsters[monsterData.id] = std::make_tuple(index++, std::move(monsterIcon), monsterData);
		}
	}


	mainGameCursor_1st.baseX = 0.145f;
	mainGameCursor_1st.baseY = 0.947f;
	mainGameCursor_1st.position = DirectX::XMFLOAT3(mainGameCursor_1st.baseX * curWinSize->width, mainGameCursor_1st.baseY * curWinSize->height, 0.0f);
	mainGameCursor_1st.size = DirectX::XMFLOAT3(250, 50, 1.0f);
	mainGameCursor_1st.curIdx = 0;
	mainGameCursor_1st.spacing = GENERAL_BUTTON_SPACING;
	if (mainGameCursor_1st.textureRect == nullptr)
		mainGameCursor_1st.textureRect = new CursorTextureRect(mainGameCursor_1st.position,
			mainGameCursor_1st.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	mainGameCursor_2nd.baseX = 0.665f;
	mainGameCursor_2nd.baseY = 0.91f;
	mainGameCursor_2nd.position = DirectX::XMFLOAT3(mainGameCursor_2nd.baseX * curWinSize->width, mainGameCursor_2nd.baseY * curWinSize->height, 0.0f);
	mainGameCursor_2nd.size = DirectX::XMFLOAT3(620, 100, 1.0f);
	mainGameCursor_2nd.curIdx = 0;
	mainGameCursor_2nd.spacing = MONSTER_STATUS_SPACING;
	if (mainGameCursor_2nd.textureRect == nullptr)
		mainGameCursor_2nd.textureRect = new CursorTextureRect(mainGameCursor_2nd.position,
			mainGameCursor_2nd.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	mainGameCursor_1st.textureRect->UpdateSize(mainGameCursor_1st.size);
	mainGameCursor_2nd.textureRect->UpdateSize(mainGameCursor_2nd.size);
	mainGameCursor_1st.Update(mainGameCursor_1st.curIdx);
	mainGameCursor_2nd.Update(mainGameCursor_2nd.curIdx);

	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(0, 0, 0);
	if (rightDispalyBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth * 2 / 3, WinMaxHeight / 2, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth * 2 / 3, WinMaxHeight, 1);
		rightDispalyBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (leftButtonBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight * 3 / 4, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 2, 1);
		leftButtonBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (timerBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight * 3 / 8, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 4, 1);
		timerBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (walkingStepBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight / 8, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 4, 1);
		walkingStepBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}


	gameMenuButtons.clear();
	gameMenuButtons.push_back(std::make_unique<Button>(L"資料", DirectX::XMFLOAT3(0.035f, 0.025f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr, true));
	gameMenuButtons.push_back(std::make_unique<Button>(L"アイテム", DirectX::XMFLOAT3(0.035f, 0.1f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::itemCheck));
	gameMenuButtons.push_back(std::make_unique<Button>(L"セーブ", DirectX::XMFLOAT3(0.035f, 0.175f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::saveGameUI));
	gameMenuButtons.push_back(std::make_unique<Button>(L"ロード", DirectX::XMFLOAT3(0.035f, 0.25f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::loadGameUI));
	gameMenuButtons.push_back(std::make_unique<Button>(L"タイトルへ戻る", DirectX::XMFLOAT3(0.035f, 0.325f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::title));
	gameMenuButtons.push_back(std::make_unique<Button>(L"ゲーム終了", DirectX::XMFLOAT3(0.035f, 0.40f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
}

void GameUI::InitPlayerState()
{
	// player icon
	if (playerIcon == nullptr)
	{
		DirectX::XMFLOAT3 playerIonPosition = DirectX::XMFLOAT3(TileWidth - 20, WinMaxHeight - TileHeight * 2 + 20, 0);
		DirectX::XMFLOAT3 playerIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
		Texture2D* playerIconTex = new Texture2D(PicturesPath + L"1.png");
		playerIcon = new TextureRect(playerIonPosition, playerIonSize, 0.0f);
		playerIcon->SetSRV(playerIconTex->GetSRV());
	}

	// yellow key icon
	if (yellowKeyIcon == nullptr)
	{
		DirectX::XMFLOAT3 yellowKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 15, WinMaxHeight - 525, 0);
		DirectX::XMFLOAT3 yellowKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
		Texture2D* yellowKeyIconTex = new Texture2D(IconsPath + L"黄色の鍵.png");
		yellowKeyIcon = new TextureRect(yellowKeyIonPosition, yellowKeyIonSize, 0.0f);
		yellowKeyIcon->SetSRV(yellowKeyIconTex->GetSRV());
	}


	// blue key icon
	if (blueKeyIcon == nullptr)
	{
		DirectX::XMFLOAT3 blueKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 15, WinMaxHeight - 575, 0);
		DirectX::XMFLOAT3 blueKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
		Texture2D* blueKeyIconTex = new Texture2D(IconsPath + L"紫の鍵.png");
		blueKeyIcon = new TextureRect(blueKeyIonPosition, blueKeyIonSize, 0.0f);
		blueKeyIcon->SetSRV(blueKeyIconTex->GetSRV());
	}

	// red key icon
	if (redKeyIcon == nullptr)
	{
		DirectX::XMFLOAT3 redKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 15, WinMaxHeight - 625, 0);
		DirectX::XMFLOAT3 redKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
		Texture2D* redKeyIconTex = new Texture2D(IconsPath + L"赤い鍵.png");
		redKeyIcon = new TextureRect(redKeyIonPosition, redKeyIonSize, 0.0f);
		redKeyIcon->SetSRV(redKeyIconTex->GetSRV());
	}

	// green key icon
	if (greenKeyIcon == nullptr)
	{
		DirectX::XMFLOAT3 greenKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 15, WinMaxHeight - 675, 0);
		DirectX::XMFLOAT3 greenKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
		Texture2D* greenKeyIconTex = new Texture2D(IconsPath + L"greenkey.png");
		greenKeyIcon = new TextureRect(greenKeyIonPosition, greenKeyIonSize, 0.0f);
		greenKeyIcon->SetSRV(greenKeyIconTex->GetSRV());
	}

}

void GameUI::InitItemCheck()
{
	DirectX::XMFLOAT3 position{}, size;
	if (itemCheckTitleBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 8, WinMaxHeight * (1 - 0.07), 0);
		size = DirectX::XMFLOAT3(WinMaxWidth / 4, WinMaxHeight * 0.13, 1);
		itemCheckTitleBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (itemCheckCategoryBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 8, position.y - WinMaxHeight * 0.86 / 2, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth / 4, WinMaxHeight * 0.72, 1);
		itemCheckCategoryBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (itemCheckDetailBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth * 5 / 8, WinMaxHeight * 0.569, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth * 0.75, WinMaxHeight * 0.85, 1);
		itemCheckDetailBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	if (itemCheckDescriptionBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 2, WinMaxHeight * 0.07, 0);
		size = DirectX::XMFLOAT3(WinMaxWidth, WinMaxHeight * 0.13, 1);
		itemCheckDescriptionBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	

	mainGameCursor_1st.baseX = 0.125f;
	mainGameCursor_1st.baseY = 0.797f;
	mainGameCursor_1st.position = DirectX::XMFLOAT3(mainGameCursor_1st.baseX * curWinSize->width, mainGameCursor_1st.baseY * curWinSize->height, 0.0f);
	mainGameCursor_1st.size = DirectX::XMFLOAT3(210, 50, 1.0f);
	mainGameCursor_1st.curIdx = 0;
	mainGameCursor_1st.spacing = GENERAL_BUTTON_SPACING;
	mainGameCursor_1st.textureRect->UpdateSize(mainGameCursor_1st.size);
	mainGameCursor_1st.Update(mainGameCursor_1st.curIdx);

	mainGameCursor_2nd.baseX = 0.625f;
	mainGameCursor_2nd.baseY = 0.927f;
	mainGameCursor_2nd.position = DirectX::XMFLOAT3(mainGameCursor_2nd.baseX * curWinSize->width, mainGameCursor_2nd.baseY * curWinSize->height, 0.0f);
	mainGameCursor_2nd.size = DirectX::XMFLOAT3(690, 50, 1.0f);
	mainGameCursor_2nd.curIdx = 0;
	mainGameCursor_2nd.spacing = GENERAL_BUTTON_SPACING;
	mainGameCursor_2nd.textureRect->UpdateSize(mainGameCursor_2nd.size);
	mainGameCursor_2nd.Update(mainGameCursor_2nd.curIdx);
	
	// clear current item buttons
	itemCategoryButtons.clear();
	itemButtons.clear();
	itemButtonIdx.clear();
	curItemCategory.clear();
	UINT itemCategoryIdx = 0;
	for (auto& item : Player::player->GetItems())
	{
		// skip zero count item
		if (item.second == 0) continue;

		std::wstring itemCategory = std::get<0>(Database::itemCategoryMap[item.first]);
		if (itemButtons.find(itemCategory) == itemButtons.end())
		{
			if (curItemCategory.empty()) curItemCategory = itemCategory;
			std::unique_ptr itemButton = std::make_unique<Button>(
				itemCategory, DirectX::XMFLOAT3(0.04f, 0.175f + mainGameCursor_2nd.spacing * itemCategoryIdx++, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr, true);
			itemButton.get()->overrideOnClick(ButtonOnClick::showItemList, itemCategory);
			itemCategoryButtons.push_back(std::move(itemButton));
			 
		}
		// item button
		std::wstring itemName = std::get<1>(Database::itemCategoryMap[item.first]);
		std::unique_ptr<ItemCategoryButton> itemButton = std::make_unique<ItemCategoryButton>(
			itemName, DirectX::XMFLOAT3(0.34f, 0.045f + mainGameCursor_2nd.spacing * itemButtonIdx[itemCategory]++, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr, nullptr, true);
		// item button text
		DirectX::XMFLOAT3 itemIonPosition = DirectX::XMFLOAT3(0, 0, 0);
		DirectX::XMFLOAT3 itemIonSize = DirectX::XMFLOAT3(TileWidth * 0.8f, TileHeight * 0.8f, 1);
		Texture2D* itemIconTex = new Texture2D(IconsPath + itemName + L".png");
		std::unique_ptr<TextureRect> itemIcon = std::make_unique<TextureRect>(itemIonPosition, itemIonSize, 0.0f);
		itemIcon->SetSRV(itemIconTex->GetSRV());
		itemButton.get()->itemIcon = std::move(itemIcon);
		itemButton.get()->UpdateCount(item.second);
		itemButton.get()->pOnClick = Database::itemButtonFuncMap[item.first];
		if (itemButton.get()->pOnClick == nullptr)
		{
			itemButton.get()->itemIcon->SetEnable(false);
		}
		itemButtons[itemCategory].push_back(std::move(itemButton));

		
	}

	UIstate = UIState::MenuLevel1;
	mainGameCursor_1st.enabled = true;
	mainGameCursor_1st.textureRect->SetCursorEnable(true);
}

void GameUI::InitSaveLoadData()
{
	DirectX::XMFLOAT3 position, size;
	if (saveDataTitleBase == nullptr)
	{
		position = DirectX::XMFLOAT3(WinMaxWidth / 2, WinMaxHeight * (1-0.07), 0);
		size = DirectX::XMFLOAT3(WinMaxWidth, WinMaxHeight * 0.136f, 1);
		saveDataTitleBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	}
	
	if (saveSlotBases.empty())
	{
		float saveSlotBaseX = WinMaxHeight * 0.748;
		float spacing = WinMaxHeight * SAVE_SLOTS_SPACING;
		for (int i = 0; i < SAVE_SLOTS_PER_PAGE; i++)
		{
			position = DirectX::XMFLOAT3(WinMaxWidth / 2, saveSlotBaseX - spacing * i, 0);
			size = DirectX::XMFLOAT3(WinMaxWidth, WinMaxHeight * SAVE_SLOTS_SPACING, 1);
			saveSlotBases.push_back(new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png"));
		}
		
	}

	saveDataButtons.clear();
	for (int i = 0; i < SAVE_SLOT_SIZE; i++)
	{
		std::unique_ptr<Button> saveDataButton = std::make_unique<Button>(L"ファイル " + std::to_wstring(i), DirectX::XMFLOAT3(0.035f, 0.025f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr);
		if (renderMode == SAVEDATA)
		{
			saveDataButton.get()->overrideOnClick(&ButtonOnClick::saveData, i);
			saveDataButtons.push_back(std::move(saveDataButton));
		}
			
		else if (renderMode == LOADDATA)
		{
			saveDataButton.get()->overrideOnClick(&ButtonOnClick::loadData, i);
			saveDataButtons.push_back(std::move(saveDataButton));
		}
			
	}
	saveDataButtons[0].get()->isSelected = true;

	if (mainGameCursor_1st.textureRect == nullptr)
		mainGameCursor_1st.textureRect = new CursorTextureRect(mainGameCursor_1st.position,
			mainGameCursor_1st.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	mainGameCursor_1st.baseX = 0.086f;
	mainGameCursor_1st.baseY = 0.81f;
	mainGameCursor_1st.position = DirectX::XMFLOAT3(mainGameCursor_1st.baseX * curWinSize->width, mainGameCursor_1st.baseY * curWinSize->height, 0.0f);
	mainGameCursor_1st.size = DirectX::XMFLOAT3(140, 46, 1.0f);
	mainGameCursor_1st.curIdx = 0;
	mainGameCursor_1st.spacing = SAVE_SLOTS_SPACING;
	mainGameCursor_1st.textureRect->UpdateSize(mainGameCursor_1st.size);
	mainGameCursor_1st.Update(mainGameCursor_1st.curIdx);

	mainGameCursor_1st.enabled = true;
	mainGameCursor_1st.textureRect->SetCursorEnable(true);

	playerIcons.resize(SAVE_SLOTS_PER_PAGE);
}

void GameUI::InitDialogue()
{
	// restrict player movement
	Player::player->SetAllowControl(false);

	// init UI base
	DirectX::XMFLOAT3 position{}, size{};
	position = DirectX::XMFLOAT3(WinMaxWidth * 0.622, WinMaxHeight * 0.2, 0);
	size = DirectX::XMFLOAT3(WinMaxWidth * 0.745, WinMaxHeight * 0.35, 1);
	dialogueBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	dialogueBase->SetDialogue(true);
	position = DirectX::XMFLOAT3(WinMaxWidth * 0.3, WinMaxHeight * 0.42, 0);
	size = DirectX::XMFLOAT3(WinMaxWidth * 0.11, WinMaxHeight * 0.09, 1);
	nameBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	nameBase->SetDialogue(true);
}

void GameUI::InitMessage()
{
	// restrict player movement
	Player::player->SetAllowControl(false);

	// init UI base
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(WinMaxWidth * 0.622, WinMaxHeight * 0.5, 0);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(WinMaxWidth * 0.745, WinMaxHeight * 0.35, 1);
	dialogueBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	dialogueBase->SetDialogue(true);
}

void GameUI::InitWarp()
{
	InitMessage();

	if (warpCursor.textureRect == nullptr)
		warpCursor.textureRect = new CursorTextureRect(warpCursor.position,
			warpCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	warpCursor.baseX = 0.3f;
	warpCursor.baseY = 0.545f;
	warpCursor.position = DirectX::XMFLOAT3(warpCursor.baseX * curWinSize->width, warpCursor.baseY * curWinSize->height, 0.0f);
	warpCursor.size = DirectX::XMFLOAT3(30, 46, 1.0f);
	warpCursor.curIdx = 0;
	warpCursor.vertical = false;
	warpCursor.spacing = WARP_FLOOR_SPACING;
	warpCursor.textureRect->UpdateSize(warpCursor.size);
	warpCursor.Update(warpCursor.curIdx);

	warpCursor.enabled = true;
	warpCursor.textureRect->SetCursorEnable(true);
	int curFloor = Player::player->GetCurFloor();
	for (int i = 0; i < 3; i++)
	{
		
		std::unique_ptr warpButton = std::make_unique<Button>(
			std::to_wstring(curFloor % 10), DirectX::XMFLOAT3(0.29f + warpCursor.spacing * i, 0.425f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr, true);
		curFloor /= 10;
		warpButtons.push_back(std::move(warpButton));
	}


}

// Render method to render UI based on the current render mode
void GameUI::Render()
{
	switch (renderMode)
	{
	case STARTMENU:
		base->Render();
		for (int i = 0; i < startButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				startButtons[i]->text.c_str(),
				startButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				startButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		startCursor.textureRect->Render();
		break;

	case INGAMEUI:
	{
		walkingStepBase->Render();
		rightDispalyBase->Render();
		leftButtonBase->Render();
		timerBase->Render();

		RenderInGameUI();

		mainGameCursor_1st.textureRect->Render();
		if (UIstate == UIState::MenuLevel2) mainGameCursor_2nd.textureRect->Render();
		RenderMonsterStates();
		break;
	}

	case PLAYERSTATES:
		RenderPlayerStates();
		break;

	case ITEMCHECK:
	{
		itemCheckTitleBase->Render();
		itemCheckCategoryBase->Render();
		itemCheckDetailBase->Render();
		itemCheckDescriptionBase->Render();

		RenderItemCheck();

		mainGameCursor_1st.textureRect->Render();
		if (UIstate == UIState::MenuLevel2) mainGameCursor_2nd.textureRect->Render();
		break;
	}

	case SAVEDATA:
	case LOADDATA:
	{
		RenderSaveLoadUI(renderMode);
		break;
	}

	case DIALOGUE:
	{
		if (!dialogueName.empty())
			nameBase->Render();
		dialogueBase->Render();

		D2D1_RECT_F TextRect = GetTextRect(DIALOGUE_RECT_LEFT, DIALOGUE_RECT_TOP, DIALOGUE_RECT_RIGHT, DIALOGUE_RECT_BOTTOM);

		mD2DResource->pD2DRenderTarget->DrawText(
			dialogueText.c_str(),
			dialogueText.length(),
			mD2DResource->pTextFormat,
			TextRect,
			mD2DResource->pSolidColorBrush);

		TextRect = GetTextRect(DIALOGUE_RECT_LEFT, DIALOGUE_NAME_RECT_TOP, DIALOGUE_RECT_NAME_RIGHT, DIALOGUE_NAME_RECT_BOTTOM);

		mD2DResource->pD2DRenderTarget->DrawText(
			dialogueName.c_str(),
			dialogueName.length(),
			mD2DResource->pTextFormat,
			TextRect,
			mD2DResource->pSolidColorBrush);

		for (int i = 0; i < dialogueButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				dialogueButtons[i]->text.c_str(),
				dialogueButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				dialogueButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		if (dialogueCursor.textureRect) dialogueCursor.textureRect->Render();
		break;
	}

	case MESSAGE:
	{
		dialogueBase->Render();

		D2D1_RECT_F TextRect = GetTextRect(DIALOGUE_RECT_LEFT, DIALOGUE_ITEM_RECT_TOP, DIALOGUE_RECT_RIGHT, DIALOGUE_ITEM_RECT_BOTTOM);

		mD2DResource->pD2DRenderTarget->DrawText(
			dialogueText.c_str(),
			dialogueText.length(),
			mD2DResource->pTextFormat,
			TextRect,
			mD2DResource->pSolidColorBrush);

		break;
	}
	case WARP:
	{
		dialogueBase->Render();

		D2D1_RECT_F TextRect = GetTextRect(DIALOGUE_RECT_LEFT, DIALOGUE_ITEM_RECT_TOP, DIALOGUE_RECT_RIGHT, DIALOGUE_ITEM_RECT_BOTTOM);

		mD2DResource->pD2DRenderTarget->DrawText(
			dialogueText.c_str(),
			dialogueText.length(),
			mD2DResource->pTextFormat,
			TextRect,
			mD2DResource->pSolidColorBrush);
		int curFloor = Player::player->GetCurFloor();
		for (int i = warpButtons.size() - 1; i >= 0; i--)
		{
			int digit = curFloor % 10;
			curFloor /= 10;
			warpButtons[i]->text = std::to_wstring(digit);
		}
		for (int i = 0; i < warpButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				warpButtons[i]->text.c_str(),
				warpButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				warpButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		warpCursor.textureRect->Render();

		break;
	}

	default:
		break;
	}	

}

void GameUI::Update()
{
	switch (renderMode)
	{
	case STARTMENU:
		base->Update();
		UpdateCursorAndButton(startCursor, startButtons);
		break;
	case INGAMEUI:

		// base UI
		rightDispalyBase->Update();
		leftButtonBase->Update();
		timerBase->Update();
		walkingStepBase->Update();

		// monster Icon
		for (auto& monster : monsters)
		{
			if (std::get<0>(monster.second) >= 0 && std::get<0>(monster.second) <= 7)
			{
				std::get<1>(monster.second)->Update();
			}
		}

		// button and cursor
		UpdateCursorAndButton(mainGameCursor_1st, gameMenuButtons);

		// UIState
		UpdateUIState();
			
		break;
	case PLAYERSTATES:
		playerIcon->Update();
		yellowKeyIcon->Update();
		blueKeyIcon->Update();
		redKeyIcon->Update();
		greenKeyIcon->Update();
		break;

	case ITEMCHECK:
		itemCheckTitleBase->Update();
		itemCheckCategoryBase->Update();
		itemCheckDetailBase->Update();
		itemCheckDescriptionBase->Update();

		if (UIstate == UIState::MenuLevel1)
		{
			UpdateCursorAndButton(mainGameCursor_1st, itemCategoryButtons);
			std::vector<std::unique_ptr<ItemCategoryButton>>& buttonRef = itemButtons[curItemCategory];
			UINT position = 0;
			for (auto& button : buttonRef)
			{
				button.get()->itemIcon.get()->UpdatePosition(DirectX::XMFLOAT3(280, WinMaxHeight - 75 - position++ * 57, 0));
				button.get()->itemIcon.get()->Update();
			}
		}
			
		else if (UIstate == UIState::MenuLevel2)
		{
			std::vector<std::unique_ptr<Button>>& buttonRef = reinterpret_cast<std::vector<std::unique_ptr<Button>>&>(itemButtons[curItemCategory]);
			UpdateCursorAndButton(mainGameCursor_2nd, buttonRef);
		}
			
		UpdateUIState();
		break;

	case SAVEDATA:
	case LOADDATA:
	{
		saveDataTitleBase->Update();
		for (int i = 0; i < SAVE_SLOTS_PER_PAGE; i++)
			saveSlotBases[i]->Update();

		// update save data buttons for current page
		int start = curSaveSlotPage * SAVE_SLOTS_PER_PAGE;
		std::vector<std::unique_ptr<Button>> curPageButtons;
		for (int i = 0; i < SAVE_SLOTS_PER_PAGE; ++i)
		{
			curPageButtons.push_back(std::move(saveDataButtons[start + i]));
		}
		UpdateSaveSlotCursor(mainGameCursor_1st, curPageButtons);
		// Move back the elements to data after processing
		for (int i = 0; i < SAVE_SLOTS_PER_PAGE; ++i)
		{
			saveDataButtons[start + i] = std::move(curPageButtons[i]);
		}

		// in game menu switch
		if (!keyboard.Press('X'))
			mApp.SetAllowSwitch(true);
		if (keyboard.Down('X') && mApp.AvailableToSwitch())
		{
			// reset cursor index
			mainGameCursor_1st.curIdx = 0;
			mainGameCursor_1st.Update(mainGameCursor_1st.curIdx);
			// prvent from switching back to UI mode
			mApp.SetAllowSwitch(false);
			// switch game mode
			if (mApp.GetGameMode() == GameMode::TITLE)
			{
				mApp.SetGameMode(GameMode::TITLE);
				mApp.SetValidGO(L"StartMenuGO", true);
				mApp.SetValidGO(L"GameUIGO", false);
			}
				
			else
				mApp.SetGameMode(GameMode::GAMEPLAY);

		}
		break;
	}

	case DIALOGUE:
		if (!dialogueName.empty())
			nameBase->Update();
		dialogueBase->Update();

		if (dialogueButtons.size() != 0) UpdateCursorAndButton(dialogueCursor, dialogueButtons);

		if (keyboard.Up('C') ||
			keyboard.Up(VK_RETURN) ||
			keyboard.Up(VK_SPACE))
		{
			mApp.DestroyGO(L"UIDialogueGO");
			Player::player->SetAllowControl(true);
		}
		break;
	case MESSAGE:
		dialogueBase->Update();

		if (keyboard.Up('C') ||
			keyboard.Up(VK_RETURN) ||
			keyboard.Up(VK_SPACE))
		{
			allowChange = true;
			break;
		}

		if (allowChange && (keyboard.Press('C') ||
			keyboard.Press(VK_RETURN) ||
			keyboard.Press(VK_SPACE)))
		{
			mApp.DestroyGO(L"UIDialogueGO");
			Player::player->SetAllowControl(true);
		}
		break;
	case WARP:
	{
		dialogueBase->Update();
		UpdateWarpCursor(warpCursor, warpButtons);
		break;
	}
	default:
		break;
	}
}

void GameUI::UpdateSaveSlotCursor(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons)
{
	// disable button and cursor when changing game mode
	if (gameModeOnChanging)	return;


	curTime = Timer::TotalTime();
	if (cursor.isPressed)
	{
		// wait for key up
		if (keyboard.Up(VK_UP))
		{
			if (cursor.curIdx == 0)
			{
				curSaveSlotPage = (curSaveSlotPage - 1 + totalSaveSlotPages) % totalSaveSlotPages;
			}
			cursor.MoveUp(buttons);
			cursor.isPressed = false;

		}
		else if (keyboard.Up(VK_DOWN))
		{
			if (cursor.curIdx == SAVE_SLOTS_PER_PAGE - 1)
			{
				curSaveSlotPage = (curSaveSlotPage + 1) % totalSaveSlotPages;
			}
			cursor.MoveDown(buttons);
			cursor.isPressed = false;

		}

		// if still pressed, move cursor
		curTime = Timer::TotalTime();
		if (curTime - lastTime > cursor.cursorMoveTimeInterval)
		{
			lastTime = curTime;
			if (cursor.moveDirection == Cursor::Direction::UP)
			{
				cursor.MoveUp(buttons);
			}

			else
			{
				cursor.MoveDown(buttons);
			}

		}
	}
	else if (keyboard.Press(VK_UP))
	{

		cursor.moveDirection = Cursor::Direction::UP;
		cursor.isPressed = true;
		lastTime = curTime;
	}
	else if (keyboard.Press(VK_DOWN))
	{
		cursor.moveDirection = Cursor::Direction::DOWN;
		cursor.isPressed = true;
		lastTime = curTime;
	}

	if (keyboard.Down('C') ||
		keyboard.Down(VK_RETURN) ||
		keyboard.Down(VK_SPACE))
	{
		cursor.Execute(buttons);
	}

	cursor.textureRect->Update();
}

void GameUI::UpdateMonsterCursor(Cursor& cursor)
{
	curTime = Timer::TotalTime();
	if (cursor.isPressed)
	{
		// wait for key up
		if (keyboard.Up(VK_UP))
		{
			if (cursor.curIdx == 0)
			{
				int countUp = 0;
				for (auto it = monsters.begin(); it != monsters.end(); ++it)
				{
					if (std::get<0>(it->second) < countUp++)
						std::get<0>(it->second) += 1;
					else
						break;
				}

			}
			cursor.MoveUp();
			cursor.isPressed = false;
		}
		else if (keyboard.Up(VK_DOWN))
		{

			int maxCount = 0;
			if (monsters.size() != 0)
				maxCount = std::get<0>(monsters.rbegin()->second) > MAX_MONSTER_STATE_COUNT ? MAX_MONSTER_STATE_COUNT: std::get<0>(monsters.rbegin()->second);
			if (cursor.curIdx == MAX_MONSTER_STATE_COUNT)
			{
				int countDown = MAX_MONSTER_STATE_COUNT;
				for (auto rit = monsters.rbegin(); rit != monsters.rend(); ++rit)
				{
					if (std::get<0>(rit->second) > countDown--)
						std::get<0>(rit->second) -= 1;
					else
						break;
				}
			}
			cursor.MoveDown(maxCount);
			cursor.isPressed = false;
		}

		// if still pressed, move cursor
		curTime = Timer::TotalTime();
		if (curTime - lastTime > cursor.cursorMoveTimeInterval)
		{
			lastTime = curTime;
			if (cursor.moveDirection == Cursor::Direction::UP)
			{
				if (cursor.curIdx == 0)
				{
					int countUp = 0;
					for (auto it = monsters.begin(); it != monsters.end(); ++it)
					{
						if (std::get<0>(it->second) < countUp++)
							std::get<0>(it->second) += 1;
						else
							break;
					}

				}
				cursor.MoveUp();
			}
				
			else 
			{
				int maxCount = 0;
				if (monsters.size() != 0)
					maxCount = std::get<0>(monsters.rbegin()->second) > MAX_MONSTER_STATE_COUNT ? MAX_MONSTER_STATE_COUNT : std::get<0>(monsters.rbegin()->second);
				if (cursor.curIdx == MAX_MONSTER_STATE_COUNT)
				{
					int countDown = MAX_MONSTER_STATE_COUNT;
					for (auto rit = monsters.rbegin(); rit != monsters.rend(); ++rit)
					{
						if (std::get<0>(rit->second) > countDown--)
							std::get<0>(rit->second) -= 1;
						else
							break;
					}
				}
				cursor.MoveDown(maxCount);
			}
				
		}
	}
	else if (keyboard.Press(VK_UP))
	{

		cursor.moveDirection = Cursor::Direction::UP;
		cursor.isPressed = true;
		lastTime = curTime;
	}
	else if (keyboard.Press(VK_DOWN))
	{
		cursor.moveDirection = Cursor::Direction::DOWN;
		cursor.isPressed = true;
		lastTime = curTime;
	}

	cursor.textureRect->Update();
}

void GameUI::UpdateUIState()
{
	if (renderModeOnChanging)
	{
		renderModeOnChanging = false;
		return;
	}
	switch (UIstate)
	{
	case UIState::MenuLevel1:
		// in game menu switch
		if (!keyboard.Press('X'))
			mApp.SetAllowSwitch(true);
		if (keyboard.Down('X') && mApp.AvailableToSwitch())
		{
			// reset cursor index
			mainGameCursor_1st.curIdx = 0;
			mainGameCursor_2nd.curIdx = 0;
			mainGameCursor_1st.Update(mainGameCursor_1st.curIdx);
			mainGameCursor_2nd.Update(mainGameCursor_2nd.curIdx);
			// prvent from switching back to UI mode
			mApp.SetAllowSwitch(false);
			// switch to gameplay mode
			mApp.SetGameMode(GameMode::GAMEPLAY);
		}
		else if (keyboard.Down('C') || keyboard.Down(VK_RETURN) || keyboard.Down(VK_SPACE))
		{
			UIstate = UIState::MenuLevel2;
			mainGameCursor_1st.enabled = false;
			mainGameCursor_2nd.enabled = true;
			mainGameCursor_1st.textureRect->SetCursorEnable(false);
		}

		break;
	case UIState::MenuLevel2:
		if (renderMode == UIRenderMode::INGAMEUI)	UpdateMonsterCursor(mainGameCursor_2nd);
		if (keyboard.Press('X'))
		{
			// reset cursor index
			mainGameCursor_2nd.curIdx = 0;
			mainGameCursor_2nd.Update(mainGameCursor_2nd.curIdx);
			UIstate = UIState::MenuLevel1;
			mainGameCursor_1st.enabled = true;
			mainGameCursor_1st.textureRect->SetCursorEnable(true);
		}
		break;
	}
}

void GameUI::UpdateWarpCursor(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons)
{
	// disable button and cursor when changing game mode
	if (gameModeOnChanging) return;

	cursor.textureRect->Update();

	curTime = Timer::TotalTime();
	if (cursor.enabled)
	{
		if (cursor.isPressed)
		{
			// wait for key up
			if (keyboard.Up(VK_LEFT))
			{
				cursor.MoveUp(buttons);
				cursor.isPressed = false;
			}
			else if (keyboard.Up(VK_RIGHT))
			{
				cursor.MoveDown(buttons);
				cursor.isPressed = false;
			}
			else if (keyboard.Up(VK_UP))
			{
				int num = std::stoi(buttons[cursor.curIdx].get()->text);
				if (++num == 10)
					num = 0;
				buttons[cursor.curIdx].get()->text = std::to_wstring(num);
				cursor.isPressed = false;
			}
			else if (keyboard.Up(VK_DOWN))
			{
				int num = std::stoi(buttons[cursor.curIdx].get()->text);
				if (--num == -1)
					num = 9;
				buttons[cursor.curIdx].get()->text = std::to_wstring(num);
				cursor.isPressed = false;
			}

			// if still pressed, move cursor
			curTime = Timer::TotalTime();
			if (curTime - lastTime > cursor.cursorMoveTimeInterval)
			{
				lastTime = curTime;
				if (cursor.moveDirection == Cursor::Direction::LEFT)
					cursor.MoveUp(buttons);
				else if (cursor.moveDirection == Cursor::Direction::RIGHT)
					cursor.MoveDown(buttons);
				else if (cursor.moveDirection == Cursor::Direction::UP)
				{
					int num = std::stoi(buttons[cursor.curIdx].get()->text);
					if (++num == 10)
						num = 0;
					buttons[cursor.curIdx].get()->text = std::to_wstring(num);
				}
				else if (cursor.moveDirection == Cursor::Direction::DOWN)
				{
					int num = std::stoi(buttons[cursor.curIdx].get()->text);
					if (--num == -1)
						num = 9;
					buttons[cursor.curIdx].get()->text = std::to_wstring(num);
				}
			}
		}
		else if (keyboard.Press(VK_LEFT))
		{
			cursor.moveDirection = Cursor::Direction::LEFT;
			cursor.isPressed = true;
			lastTime = curTime;
		}
		else if (keyboard.Press(VK_RIGHT))
		{
			cursor.moveDirection = Cursor::Direction::RIGHT;
			cursor.isPressed = true;
			lastTime = curTime;
		}
		else if (keyboard.Press(VK_UP))
		{
			cursor.moveDirection = Cursor::Direction::UP;
			cursor.isPressed = true;
			lastTime = curTime;
		}
		else if (keyboard.Press(VK_DOWN))
		{
			cursor.moveDirection = Cursor::Direction::DOWN;
			cursor.isPressed = true;
			lastTime = curTime;
		}

		if (keyboard.Down('C') ||
			keyboard.Down(VK_RETURN) ||
			keyboard.Down(VK_SPACE))
		{
			int floor = 0, base = 1;
			for (int i = buttons.size() - 1; i >= 0; i--)
			{
				floor  += std::stoi(buttons[i].get()->text) * base;
				base *= 10;
			}
			for (int j = 0; j < buttons.size(); j++)
				buttons[j].get()->overrideOnClick(ButtonOnClick::warp, floor);
			mApp.DestroyGO(L"UIDialogueGO");
			cursor.Execute(buttons);
		}
	}
}

void GameUI::UpdateCursorAndButton(Cursor& cursor, std::vector<std::unique_ptr<Button>>& buttons)
{
	// disable button and cursor when changing game mode
	if (gameModeOnChanging) return;

	cursor.textureRect->Update();
	if (buttons.size() == 0)
		return;

	// update item icon if current button is item categoy button
	UINT position = 0;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->UpdateTextRect(curWinSize);
		if (ItemCategoryButton* itemCategoryButton = dynamic_cast<ItemCategoryButton*>(buttons[i].get()))
		{
			itemCategoryButton->itemIcon.get()->UpdatePosition(DirectX::XMFLOAT3(280, WinMaxHeight - 75 - position++ * 57, 0));
			itemCategoryButton->itemIcon.get()->Update();
		}
	}

	curTime = Timer::TotalTime();
	if (cursor.enabled)
	{
		if (cursor.isPressed)
		{
			// wait for key up
			if (keyboard.Up(VK_UP))
			{
				cursor.MoveUp(buttons);
				cursor.isPressed = false;
			}
			else if (keyboard.Up(VK_DOWN))
			{
				cursor.MoveDown(buttons);
				cursor.isPressed = false;
			}

			// if still pressed, move cursor
			curTime = Timer::TotalTime();
			if (curTime - lastTime > cursor.cursorMoveTimeInterval)
			{
				lastTime = curTime;
				if (cursor.moveDirection == Cursor::Direction::UP)
					cursor.MoveUp(buttons);
				else
					cursor.MoveDown(buttons);
			}
		}
		else if (keyboard.Press(VK_UP))
		{
			cursor.moveDirection = Cursor::Direction::UP;
			cursor.isPressed = true;
			lastTime = curTime;
		}
		else if (keyboard.Press(VK_DOWN))
		{
			cursor.moveDirection = Cursor::Direction::DOWN;
			cursor.isPressed = true;
			lastTime = curTime;
		}

		if (keyboard.Down('C') ||
			keyboard.Down(VK_RETURN) ||
			keyboard.Down(VK_SPACE))
		{
			cursor.Execute(buttons);
		}
	}
}

void GameUI::RenderInGameUI()
{
	// buttons
	for (int i = 0; i < gameMenuButtons.size(); i++)
	{
		mD2DResource->pD2DRenderTarget->DrawText(
			gameMenuButtons[i]->text.c_str(),
			gameMenuButtons[i]->text.size(),
			mD2DResource->pTextFormat,
			gameMenuButtons[i]->textRect,
			mD2DResource->pSolidColorBrush);
	}

	// text
	std::wstring timerText = L"プレイ時間";
	D2D1_RECT_F timerTextRect = GetTextRect(0.03f, 0.52f, 0.15f, 0.62f);
	IDWriteTextFormat* textFormat = DynamicTextFormat(timerText, &timerTextRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		timerText.c_str(),
		timerText.length(),
		textFormat,
		timerTextRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);

	std::wstring timer = GetFormattedTime();
	D2D1_RECT_F timerRect = GetTextRect(0.13f, 0.62f, 0.25f, 0.72f);
	textFormat = DynamicTextFormat(timer, &timerRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		timer.c_str(),
		timer.length(),
		textFormat,
		timerRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);

	std::wstring walkingStepsText = L"歩数";
	D2D1_RECT_F walkingStepsTextRect = GetTextRect(0.01f, 0.77f, 0.15f, 0.87f);
	textFormat = DynamicTextFormat(walkingStepsText, &walkingStepsTextRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		walkingStepsText.c_str(),
		walkingStepsText.length(),
		textFormat,
		walkingStepsTextRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);

	std::wstring walkingSteps = std::to_wstring(Player::player->GetWalkingSteps());
	D2D1_RECT_F walkingStepsRect = GetTextRect(0.13f, 0.87f, 0.25f, 0.97f);
	textFormat = DynamicTextFormat(walkingSteps, &walkingStepsTextRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		walkingSteps.c_str(),
		walkingSteps.length(),
		textFormat,
		walkingStepsRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);
}

void GameUI::RenderMonsterStates()
{
	if (Player::player->GetItems().find(ItemID::BOOK) == Player::player->GetItems().end())
		return;

	UINT position = 0;
	// for text color
	bool toggle = false;
	for (auto& monster : monsters)
	{
		if (position == MAX_MONSTER_STATE_COUNT + 1)
			break;
		if (std::get<0>(monster.second) == position)
		{
			// monster icon
			std::get<1>(monster.second)->UpdatePosition(DirectX::XMFLOAT3(350, WinMaxHeight - 80 - position * MONSTER_ICON_SPACING, 0));
			std::get<1>(monster.second)->Render();

			toggle = !toggle;
			// monster status
			float spacing = MONSTER_STATUS_SPACING;
			MonsterData monsterData = std::get<2>(monster.second);

			mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			std::wstring name = monsterData.name;
			D2D1_RECT_F nameTextRect = GetTextRect(0.4082f, 0.04f + position * spacing, 0.5102f, 0.084 + position * spacing);
			bool isTransformed;
			IDWriteTextFormat* textFormat = DynamicTextFormat(name, &nameTextRect, &isTransformed);
			D2D1_MATRIX_3X2_F transform;
			mD2DResource->pD2DRenderTarget->GetTransform(&transform);
			if (isTransformed)
			{
				mD2DResource->pD2DRenderTarget->DrawText(
					name.c_str(),
					name.length(),
					textFormat,
					nameTextRect,
					mD2DResource->pSolidColorBrush);
			}
			else
				DrawTextWithSpacing(textFormat, name, &nameTextRect);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring attribute = L"普通";
			D2D1_RECT_F attributeTextRect = GetTextRect(0.4082f, 0.084f + position * spacing, 0.5102f, 0.1306 + position * spacing);
			textFormat = DynamicTextFormat(attribute, &attributeTextRect);
			DrawTextWithSpacing(textFormat, attribute, &attributeTextRect);
			SafeRelease(&textFormat);

			if (!toggle)
				mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightSkyBlue));
			else
				mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGreen));

			std::wstring HP = L"生命";
			D2D1_RECT_F HPTextRect = GetTextRect(0.5153f, 0.0333f + position * spacing, 0.5714f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(HP, &HPTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				HP.c_str(),
				HP.length(),
				textFormat,
				HPTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			std::wstring v1 = std::to_wstring(monsterData.HP);
			D2D1_RECT_F v1TextRect = GetTextRect(0.5816f, 0.0333f + position * spacing, 0.6531f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(v1, &v1TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v1.c_str(),
				v1.length(),
				textFormat,
				v1TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring attack = L"攻撃";
			D2D1_RECT_F attackTextRect = GetTextRect(0.6633f, 0.0333f + position * spacing, 0.7194f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(attack, &attackTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				attack.c_str(),
				attack.length(),
				textFormat,
				attackTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			std::wstring v2 = std::to_wstring(monsterData.attack);
			D2D1_RECT_F v2TextRect = GetTextRect(0.7296f, 0.0333f + position * spacing, 0.8010f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(v2, &v2TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v2.c_str(),
				v2.length(),
				textFormat,
				v2TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring defense = L"防御";
			D2D1_RECT_F defenseTextRect = GetTextRect(0.8112f, 0.0333f + position * spacing, 0.8673f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(defense, &defenseTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				defense.c_str(),
				defense.length(),
				textFormat,
				defenseTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			std::wstring v3 = std::to_wstring(monsterData.defense);
			D2D1_RECT_F v3TextRect = GetTextRect(0.8775f, 0.0333f + position * spacing, 0.9490f, 0.08f + position * spacing);
			textFormat = DynamicTextFormat(v3, &v3TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v3.c_str(),
				v3.length(),
				textFormat,
				v3TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring gold = L"GOLD";
			D2D1_RECT_F goldTextRect = GetTextRect(0.5153f, 0.08667f + position * spacing, 0.5714f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(HP, &HPTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				gold.c_str(),
				gold.length(),
				textFormat,
				goldTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			std::wstring v4 = std::to_wstring(monsterData.gold);
			D2D1_RECT_F v4TextRect = GetTextRect(0.5816f, 0.08667f + position * spacing, 0.6531f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(v4, &v4TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v4.c_str(),
				v4.length(),
				textFormat,
				v4TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring exp = L"EXP";
			D2D1_RECT_F expTextRect = GetTextRect(0.6633f, 0.08667f + position * spacing, 0.7194f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(exp, &expTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				exp.c_str(),
				exp.length(),
				textFormat,
				expTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			std::wstring v5 = std::to_wstring(monsterData.exp);
			//D2D1_RECT_F v5TextRect = D2D1::RectF(715, 65 + position * spacing, 785, 100 + position * spacing);
			D2D1_RECT_F v5TextRect = GetTextRect(0.7296f, 0.08667f + position * spacing, 0.8010f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(v5, &v5TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v5.c_str(),
				v5.length(),
				textFormat,
				v5TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring damageText = L"ダメージ";
			D2D1_RECT_F damageTextRect = GetTextRect(0.8112f, 0.08667f + position * spacing, 0.8673f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(damageText, &damageTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				damageText.c_str(),
				damageText.length(),
				textFormat,
				damageTextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			
			int damage = Battle::CalculateDamage(monsterData);
			std::wstring v6;
			if (damage == 0)
			{
				mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				v6 = std::to_wstring(0);
			}
			else if (damage == -1)
			{
				mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
				v6 = L"????";
			}
			else
			{
				mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::IndianRed));
				v6 = std::to_wstring(damage);
			}
			D2D1_RECT_F v6TextRect = GetTextRect(0.8775f, 0.08667f + position * spacing, 0.9490f, 0.1333f + position * spacing);
			textFormat = DynamicTextFormat(v6, &v6TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v6.c_str(),
				v6.length(),
				textFormat,
				v6TextRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			position++;
		}

	}

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
}

void GameUI::RenderItemCheck()
{
	// item category button
	for (int i = 0; i < itemCategoryButtons.size(); i++)
	{
		mD2DResource->pD2DRenderTarget->DrawText(
			itemCategoryButtons[i]->text.c_str(),
			itemCategoryButtons[i]->text.size(),
			mD2DResource->pTextFormat,
			itemCategoryButtons[i]->textRect,
			mD2DResource->pSolidColorBrush);
	}

	// item button
	for (int i = 0; i < itemButtons[curItemCategory].size(); i++)
	{
		// item texturex
		itemButtons[curItemCategory][i].get()->itemIcon.get()->Render();

		// gray unenabled button
		if (itemButtons[curItemCategory][i].get()->pOnClick == nullptr)
			mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));

		mD2DResource->pD2DRenderTarget->DrawText(
			itemButtons[curItemCategory][i]->text.c_str(),
			itemButtons[curItemCategory][i]->text.size(),
			mD2DResource->pTextFormat,
			itemButtons[curItemCategory][i]->textRect,
			mD2DResource->pSolidColorBrush);
	}
	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	// title
	std::wstring itemCheckTitle = L"アイテム";
	D2D1_RECT_F itemCheckTitleRect = GetTextRect(0.045f, 0.03f, 0.16f, 0.095f);
	IDWriteTextFormat* textFormat = DynamicTextFormat(itemCheckTitle, &itemCheckTitleRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		itemCheckTitle.c_str(),
		itemCheckTitle.length(),
		textFormat,
		itemCheckTitleRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);
}

void GameUI::RenderPlayerStates()
{
	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Pink));

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"塔"),
		2,
		mD2DResource->pTextFormat,
		D2D1::RectF(100, 40, 150, 40),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"階"),
		2,
		mD2DResource->pTextFormat,
		D2D1::RectF(220, 40, 240, 40),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"レベル"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 95, 150, 95),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"生命力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 150, 150, 150),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"攻撃力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 205, 150, 205),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(std::to_wstring(Player::player->GetCurFloor()).c_str()),
		2,
		mD2DResource->pTextFormat,
		D2D1::RectF(175, 40, 175, 40),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"防御力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 260, 150, 260),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"魔防力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 315, 150, 315),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"EXP"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 370, 150, 370),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pD2DRenderTarget->DrawText(
		(L"GOLD"),
		5,
		mD2DResource->pTextFormat,
		D2D1::RectF(35, 425, 150, 425),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	std::wstring Lv = std::to_wstring(Player::player->GetBattleData().level);
	mD2DResource->pD2DRenderTarget->DrawText(
		(Lv.c_str()),
		Lv.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(215, 95, 240, 95),
		mD2DResource->pSolidColorBrush);

	std::wstring HP = std::to_wstring(Player::player->GetBattleData().HP);
	mD2DResource->pD2DRenderTarget->DrawText(
		(HP.c_str()),
		HP.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 150, 240, 150),
		mD2DResource->pSolidColorBrush);

	std::wstring ATK = std::to_wstring(Player::player->GetBattleData().atk);
	mD2DResource->pD2DRenderTarget->DrawText(
		(ATK.c_str()),
		ATK.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 205, 240, 205),
		mD2DResource->pSolidColorBrush);

	std::wstring DEF = std::to_wstring(Player::player->GetBattleData().def);
	mD2DResource->pD2DRenderTarget->DrawText(
		(DEF.c_str()),
		DEF.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 260, 240, 260),
		mD2DResource->pSolidColorBrush);

	std::wstring MDF = std::to_wstring(Player::player->GetBattleData().mdf);
	mD2DResource->pD2DRenderTarget->DrawText(
		(MDF.c_str()),
		MDF.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 315, 240, 315),
		mD2DResource->pSolidColorBrush);

	std::wstring EXP = std::to_wstring(Player::player->GetBattleData().exp);
	mD2DResource->pD2DRenderTarget->DrawText(
		(EXP.c_str()),
		EXP.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 370, 240, 370),
		mD2DResource->pSolidColorBrush);

	std::wstring GOLD = std::to_wstring(Player::player->GetBattleData().gold);
	mD2DResource->pD2DRenderTarget->DrawText(
		(GOLD.c_str()),
		GOLD.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 425, 240, 425),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
	std::wstring yellowKeyCount;
	if (Player::player->GetItems().find(ItemID::YELLOW_KEY) == Player::player->GetItems().end())
		yellowKeyCount = std::to_wstring(0);
	else
		yellowKeyCount = std::to_wstring(Player::player->GetItems().at(ItemID::YELLOW_KEY));
	mD2DResource->pD2DRenderTarget->DrawText(
		(yellowKeyCount.c_str()),
		yellowKeyCount.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 473, 240, 473),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	std::wstring blueKeyCount;
	if (Player::player->GetItems().find(ItemID::BLUE_KEY) == Player::player->GetItems().end())
		blueKeyCount = std::to_wstring(0);
	else
		blueKeyCount = std::to_wstring(Player::player->GetItems().at(ItemID::BLUE_KEY));
	mD2DResource->pD2DRenderTarget->DrawText(
		(blueKeyCount.c_str()),
		blueKeyCount.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 523, 240, 523),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	std::wstring RedKeyCount;
	if (Player::player->GetItems().find(ItemID::RED_KEY) == Player::player->GetItems().end())
		RedKeyCount = std::to_wstring(0);
	else
		RedKeyCount = std::to_wstring(Player::player->GetItems().at(ItemID::RED_KEY));
	mD2DResource->pD2DRenderTarget->DrawText(
		(RedKeyCount.c_str()),
		RedKeyCount.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 573, 240, 573),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGreen));
	std::wstring greenKeyCount;
	if (Player::player->GetItems().find(ItemID::GREEN_KEY) == Player::player->GetItems().end())
		greenKeyCount = std::to_wstring(0);
	else
		greenKeyCount = std::to_wstring(Player::player->GetItems().at(ItemID::GREEN_KEY));
	mD2DResource->pD2DRenderTarget->DrawText(
		(greenKeyCount.c_str()),
		greenKeyCount.size(),
		mD2DResource->pTextFormat,
		D2D1::RectF(110, 623, 240, 623),
		mD2DResource->pSolidColorBrush);

	mD2DResource->pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	mD2DResource->pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	playerIcon->Render();
	yellowKeyIcon->Render();
	blueKeyIcon->Render();
	redKeyIcon->Render();
	greenKeyIcon->Render();
}

void GameUI::RenderSaveLoadUI(UIRenderMode mode)
{
	std::wstring saveDataTitle;
	if (mode == SAVEDATA)	saveDataTitle = L"このファイルにセーブデータを保存しますか";
	else if (mode == LOADDATA)	saveDataTitle = L"どのファイルをロードしますか";

	// title
	saveDataTitleBase->Render();

	D2D1_RECT_F saveDataTitleRect = GetTextRect(0.045f, 0.03f, 0.56f, 0.095f);
	IDWriteTextFormat* textFormat = DynamicTextFormat(saveDataTitle, &saveDataTitleRect);

	mD2DResource->pD2DRenderTarget->DrawText(
		saveDataTitle.c_str(),
		saveDataTitle.length(),
		textFormat,
		saveDataTitleRect,
		mD2DResource->pSolidColorBrush);
	SafeRelease(&textFormat);

	// save slots
	RenderSaveSlot();

	mainGameCursor_1st.textureRect->Render();
}

void GameUI::RenderSaveSlot()
{
	for (int i = curSaveSlotPage * SAVE_SLOTS_PER_PAGE, j = 0; j < SAVE_SLOTS_PER_PAGE; i++, j++)
	{
		saveSlotBases[j]->Render();

		saveDataButtons[i]->textRect = GetTextRect(0.02f, 0.1633f + SAVE_SLOTS_SPACING * j, 0.44f, 0.21f + SAVE_SLOTS_SPACING * j);

		mD2DResource->pD2DRenderTarget->DrawText(
			saveDataButtons[i]->text.c_str(),
			saveDataButtons[i]->text.size(),
			mD2DResource->pTextFormat,
			saveDataButtons[i]->textRect,
			mD2DResource->pSolidColorBrush);

		float totalTime = 0;
		std::wstring timestamp;
		if (SaveData::LoadPreviewData(i, totalTime, timestamp))
		{
			std::wstring timer = GetFormattedTime(totalTime);

			D2D1_RECT_F timerRect = GetTextRect(0.78f, 0.2133f + SAVE_SLOTS_SPACING * j, 0.99f, 0.26f + SAVE_SLOTS_SPACING * j);
			IDWriteTextFormat* textFormat = DynamicTextFormat(timer, &timerRect);

			mD2DResource->pD2DRenderTarget->DrawText(
				timer.c_str(),
				timer.length(),
				textFormat,
				timerRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			D2D1_RECT_F timestampRect = GetTextRect(0.67f, 0.2733f + SAVE_SLOTS_SPACING * j, 0.99f, 0.32f + SAVE_SLOTS_SPACING * j);
			textFormat = DynamicTextFormat(timestamp, &timerRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				timestamp.c_str(),
				timestamp.length(),
				textFormat,
				timestampRect,
				mD2DResource->pSolidColorBrush);
			SafeRelease(&textFormat);

			if (playerIcons[j] == nullptr)
			{
				DirectX::XMFLOAT3 playerIonPosition = DirectX::XMFLOAT3(450, 10 + SAVE_SLOTS_SPACING * curWinSize->height * (SAVE_SLOTS_PER_PAGE - j - 1), 0);
				DirectX::XMFLOAT3 playerIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
				std::unique_ptr<TextureRect> playerIcon = std::make_unique<TextureRect>(playerIonPosition, playerIonSize, 0.0f);
				Texture2D* playerIconTex = new Texture2D(PicturesPath + L"1.png");
				playerIcon->SetSRV(playerIconTex->GetSRV());
				playerIcons[j] = std::move(playerIcon);

			}
			playerIcons[j]->Update();
			playerIcons[j]->Render();

		}
	}
}

void GameUI::SetDialogueText(std::wstring text, const std::vector<DialogueButtonEvent>& events)
{
	size_t pos = 0;
	std::wstring branch = DIALOGUE_BRANCH;
	if ((pos = text.find(L'\n', 0)) != std::wstring::npos &&
		text.compare(pos + 1, branch.size(), branch) == 0)
	{
		dialogueText = text.substr(0, pos);
		size_t prev_pos = pos + 1;
		UINT spacing = GENERAL_BUTTON_SPACING;
		UINT idx = 0;

		DWRITE_TEXT_METRICS textMetrics = CalculateTextMetrics(dialogueText, (DIALOGUE_RECT_RIGHT - DIALOGUE_RECT_LEFT) * curWinSize->width);
		float dialogueButtonBaseY = DIALOGUE_RECT_TOP + textMetrics.height / curWinSize->height;

		// init cursor
		dialogueCursor.baseX = 0.417;
		dialogueCursor.baseY = 1 - dialogueButtonBaseY - 0.03;
		dialogueCursor.position = DirectX::XMFLOAT3(dialogueCursor.baseX * curWinSize->width, dialogueCursor.baseY * curWinSize->height, 0.0f);
		dialogueCursor.size = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		dialogueCursor.curIdx = 0;
		dialogueCursor.spacing = GENERAL_BUTTON_SPACING;
		if (dialogueCursor.textureRect == nullptr)
			dialogueCursor.textureRect = new CursorTextureRect(dialogueCursor.position,
				dialogueCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

		dialogueButtons.clear();
		UINT dialogueButtonIdx = 0;
		while ((pos = text.find(L'\n', prev_pos)) != std::wstring::npos)
		{
			size_t start = pos + 1;
			size_t end = text.find(L'\n', start);
			if (end == std::wstring::npos)
			{
				end = text.length();
			}
			std::unique_ptr<DialogueButton> dialogueButton = std::make_unique<DialogueButton>(text.substr(start, end - start),
				DirectX::XMFLOAT3(0.27f, dialogueButtonBaseY + dialogueCursor.spacing * idx++, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, nullptr, events[dialogueButtonIdx++]);
			dialogueCursor.textureRect->UpdateSize(DirectX::XMFLOAT3(300, 50.0f, 1.0f));
			dialogueButtons.push_back(std::move(dialogueButton));
			prev_pos = end;
		}

		// add the last part ending without \n
		if (prev_pos < text.length())
		{
			dialogueButtons.push_back(std::make_unique<Button>(text.substr(prev_pos),
				DirectX::XMFLOAT3(0.4f, 0.835f + dialogueCursor.spacing * idx, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
		}
		dialogueButtons[0].get()->isSelected = true;
	}
	else
		dialogueText = text;
}

IDWriteTextFormat* GameUI::DynamicTextFormat(const std::wstring& text, const D2D1_RECT_F* textRect, bool* isTransformed)
{
	IDWriteTextFormat* textFormat = nullptr;
	float fontSize = 26.5f;
	float stepSize = 0.5f; // Step size for adjusting font size
	D2D1_SIZE_F textSize = { 0, 0 };
	if (isTransformed)
		*isTransformed = false;

	// Iterate to adjust font size until the text fits the height of the textRect
	while (true)
	{
		if (textFormat) textFormat->Release(); // Release previous text format

		mD2DResource->pD2DWriteFactory->CreateTextFormat(
			(FontPath + L"Gabriola.ttf").c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);

		textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);  // Disable word wrapping
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // Center align text horizontally
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // Center align text vertically

		IDWriteTextLayout* textLayout = nullptr;
		mD2DResource->pD2DWriteFactory->CreateTextLayout(
			text.c_str(), text.length(), textFormat, textRect->right - textRect->left, textRect->bottom - textRect->top, &textLayout);

		DWRITE_TEXT_METRICS textMetrics;
		textLayout->GetMetrics(&textMetrics);
		textSize.width = textMetrics.width;
		textSize.height = textMetrics.height;


		textLayout->Release();

		if (textSize.height > (textRect->bottom - textRect->top)) 
		{
			fontSize -= stepSize; // Decrease font size if text height is too large
		}
		else 
		{
			break; // Font size is appropriate
		}
	}

	// Apply scaling transformation with translation to center the text
	if (textSize.width > (textRect->right - textRect->left)) 
	{
		// Calculate scaling factors
		float scaleX = (textRect->right - textRect->left) / textSize.width;
		float centerX = (textRect->left + textRect->right) / 2; // Center of the rectangle
		float textCenterX = (textSize.width * scaleX) / 2; // Center of the scaled text

		D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Translation(centerX - textCenterX * scaleX - textRect->left, 0) * D2D1::Matrix3x2F::Scale(scaleX, 1.0f, D2D1::Point2F(centerX, 0));

		// Apply the scaling transformation
		mD2DResource->pD2DRenderTarget->SetTransform(transform);

		// set the transfomr to be true
		if (isTransformed)
			*isTransformed = true;
	}

	return textFormat;
}


// Custom function to draw text with adjustable character spacing
void GameUI::DrawTextWithSpacing(IDWriteTextFormat* pTextFormat, const std::wstring& text, const D2D1_RECT_F* rect) {
	float totalTextWidth = 0;
	float maxTextHeight = 0;

	// Create text layout to measure text size
	IDWriteTextLayout* pTextLayout = nullptr;
	ThrowIfFailed(mD2DResource->pD2DWriteFactory->CreateTextLayout(text.c_str(), text.length(), pTextFormat, rect->right - rect->left, rect->bottom - rect->top, &pTextLayout));

	DWRITE_TEXT_METRICS textMetrics;
	ThrowIfFailed(pTextLayout->GetMetrics(&textMetrics));

	totalTextWidth = textMetrics.width;
	maxTextHeight = textMetrics.height;

	pTextLayout->Release();
	float scaleX = (rect->right - rect->left) / textMetrics.width;

	// Calculate extra space per gap
	float totalSpaceWidth = rect->right - rect->left - totalTextWidth;
	if (totalSpaceWidth < 0)
		totalSpaceWidth = 0;
	float extraSpacePerGap = totalSpaceWidth / (text.length() + 1); // Add 1 to include head and tail spaces

	float x = rect->left + extraSpacePerGap; // Start with initial extra space
	float y = rect->top + (rect->bottom - rect->top - maxTextHeight) / 2; // Center vertically

	// Draw each character with extra spacing
	for (size_t i = 0; i < text.length(); ++i) 
	{
		std::wstring character(1, text[i]);

		// Create text layout for each character
		ThrowIfFailed(mD2DResource->pD2DWriteFactory->CreateTextLayout(character.c_str(), character.length(), pTextFormat, rect->right - rect->left, rect->bottom - rect->top, &pTextLayout));

		// Get character metrics
		ThrowIfFailed(pTextLayout->GetMetrics(&textMetrics));

		// Draw the character
		D2D1_RECT_F charRect = D2D1::RectF(x, y, x + textMetrics.width, y + textMetrics.height);
		mD2DResource->pD2DRenderTarget->DrawText(character.c_str(), character.length(), pTextFormat, charRect, mD2DResource->pSolidColorBrush);

		x += textMetrics.width + extraSpacePerGap; // Move to the next character position
		pTextLayout->Release();
	}
	// Reset transformation matrix
	mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

// Get formatted time string in HH:MM:SS format
std::wstring GameUI::GetFormattedTime(float time) const 
{
	int hours = static_cast<int>(time / 3600);
	int minutes = static_cast<int>((time) / 60) % 60;
	int seconds = static_cast<int>(time) % 60;

	std::wstringstream ss;
	ss << std::setw(2) << std::setfill(L'0') << hours << L":"
		<< std::setw(2) << std::setfill(L'0') << minutes << L":"
		<< std::setw(2) << std::setfill(L'0') << seconds;

	return ss.str();
}

D2D1_RECT_F GameUI::GetTextRect(float left, float top, float right, float bottom)
{
	D2D1_RECT_F textRect;
	float textWidth = (right - left) * curWinSize->width;
	float textHeight = (bottom - top) * curWinSize->height;
	textRect.left = curWinSize->width * left;
	textRect.top = curWinSize->height * top;
	textRect.right = textRect.left + textWidth;
	textRect.bottom = textRect.top + textHeight;
	return textRect;
}

DWRITE_TEXT_METRICS GameUI::CalculateTextMetrics(const std::wstring& text, float maxWidth)
{
	IDWriteTextLayout* textLayout = nullptr;

	mD2DResource->pD2DWriteFactory->CreateTextLayout(
		text.c_str(),
		(UINT32)text.length(),
		mD2DResource->pTextFormat,
		maxWidth,
		FLT_MAX,
		&textLayout
	);

	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	return textMetrics;

}