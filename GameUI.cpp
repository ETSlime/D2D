#include "GameUI.h"
#include "Monster.h"

GameUI::GameUI(const D2DResource* D2DResource, const WinSize* winSize, UIRenderMode mode)
	:mD2DResource(D2DResource), curWinSize(winSize), mode(mode)
{

	switch (mode)
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
	}
}

GameUI::~GameUI(){}



void GameUI::InitMainMenu()
{
	startCursor.baseX = 0.50f;
	startCursor.baseY = 0.435f;
	startCursor.position = DirectX::XMFLOAT3(startCursor.baseX * curWinSize->width, startCursor.baseY * curWinSize->height, 0.0f);
	startCursor.size = DirectX::XMFLOAT3(300, 50, 1.0f);
	startCursor.curIdx = 0;
	startCursor.textureRect = new CursorTextureRect(startCursor.position,
		startCursor.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	startButtons.push_back(new Button(L"始めから", DirectX::XMFLOAT3(0.4f, 0.535f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startGame, true));
	startButtons.push_back(new Button(L"続きから", DirectX::XMFLOAT3(0.4f, 0.635f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	startButtons.push_back(new Button(L"ゲーム終了", DirectX::XMFLOAT3(0.4f, 0.735f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	startButtons.push_back(new Button(L"遊ぶ方法説明", DirectX::XMFLOAT3(0.4f, 0.835f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));

	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(490, 220, 0);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(400, 320, 1);
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
			DirectX::XMFLOAT3 monsterIonPosition = DirectX::XMFLOAT3(350, WinMaxHeight - 80, 0);
			DirectX::XMFLOAT3 mosterIonSize = DirectX::XMFLOAT3(TileWidth * 0.8f, TileHeight * 0.8f, 1);
			Texture2D* slimeIconTex = new Texture2D(BattlersPath + monsterData.name + L".png");
			TextureRect* monsterIcon = new TextureRect(monsterIonPosition, monsterIonPosition, 0.0f);
			monsterIcon->SetSRV(slimeIconTex->GetSRV());
			monsters[monsterData.id] = std::make_tuple(index++, new TextureRect(monsterIonPosition, monsterIonPosition, 0.0f), monsterData);
		}
	}

	mainGameCursor_1st.baseX = 0.145f;
	mainGameCursor_1st.baseY = 0.947f;
	mainGameCursor_1st.position = DirectX::XMFLOAT3(mainGameCursor_1st.baseX * curWinSize->width, mainGameCursor_1st.baseY * curWinSize->height, 0.0f);
	mainGameCursor_1st.size = DirectX::XMFLOAT3(250, 50, 1.0f);
	mainGameCursor_1st.curIdx = 0;
	mainGameCursor_1st.spacing = 0.075;
	mainGameCursor_1st.textureRect = new CursorTextureRect(mainGameCursor_1st.position,
		mainGameCursor_1st.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	mainGameCursor_2nd.baseX = 0.665f;
	mainGameCursor_2nd.baseY = 0.91f;
	mainGameCursor_2nd.position = DirectX::XMFLOAT3(mainGameCursor_2nd.baseX * curWinSize->width, mainGameCursor_2nd.baseY * curWinSize->height, 0.0f);
	mainGameCursor_2nd.size = DirectX::XMFLOAT3(620, 100, 1.0f);
	mainGameCursor_2nd.curIdx = 0;
	mainGameCursor_2nd.spacing = 0.133;
	mainGameCursor_2nd.textureRect = new CursorTextureRect(mainGameCursor_2nd.position,
		mainGameCursor_2nd.size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(WinMaxWidth * 2 / 3, WinMaxHeight / 2, 0);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(WinMaxWidth * 2 / 3, WinMaxHeight, 1);
	rightDispalyBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight * 3 / 4, 0);
	size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 2, 1);
	leftButtonBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight * 3 / 8, 0);
	size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 4, 1);
	timerBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");
	position = DirectX::XMFLOAT3(WinMaxWidth / 6, WinMaxHeight / 8, 0);
	size = DirectX::XMFLOAT3(WinMaxWidth / 3, WinMaxHeight / 4, 1);
	walkingStepBase = new UITextureRect(position, size, 0.0f, SkinsPath + L"WS-prefix100-original2.png");

	gameMenuButtons.push_back(new Button(L"資料", DirectX::XMFLOAT3(0.035f, 0.025f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::startGame, true));
	gameMenuButtons.push_back(new Button(L"アイテム", DirectX::XMFLOAT3(0.035f, 0.1f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	gameMenuButtons.push_back(new Button(L"セーブ", DirectX::XMFLOAT3(0.035f, 0.175f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::exitGame));
	gameMenuButtons.push_back(new Button(L"ロード", DirectX::XMFLOAT3(0.035f, 0.25f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	gameMenuButtons.push_back(new Button(L"タイトルへ戻る", DirectX::XMFLOAT3(0.035f, 0.325f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
	gameMenuButtons.push_back(new Button(L"ゲーム終了", DirectX::XMFLOAT3(0.035f, 0.40f, 0.0f), DirectX::XMFLOAT2(500, 200), curWinSize, &ButtonOnClick::tutorial));
}

void GameUI::InitPlayerState()
{
	// player icon
	DirectX::XMFLOAT3 playerIonPosition = DirectX::XMFLOAT3(TileWidth - 20, WinMaxHeight - TileHeight * 2 + 20, 0);
	DirectX::XMFLOAT3 playerIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	Texture2D* playerIconTex = new Texture2D(PicturesPath + L"1.png");
	playerIcon = new TextureRect(playerIonPosition, playerIonSize, 0.0f);
	playerIcon->SetSRV(playerIconTex->GetSRV());

	// yellow key icon
	DirectX::XMFLOAT3 yellowKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 10, WinMaxHeight - 510, 0);
	DirectX::XMFLOAT3 yellowKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	Texture2D* yellowKeyIconTex = new Texture2D(IconsPath + L"yellowkey.png");
	yellowKeyIcon = new TextureRect(yellowKeyIonPosition, yellowKeyIonSize, 0.0f);
	yellowKeyIcon->SetSRV(yellowKeyIconTex->GetSRV());

	// blue key icon
	DirectX::XMFLOAT3 blueKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 10, WinMaxHeight - 560, 0);
	DirectX::XMFLOAT3 blueKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	Texture2D* blueKeyIconTex = new Texture2D(IconsPath + L"bluekey.png");
	blueKeyIcon = new TextureRect(blueKeyIonPosition, blueKeyIonSize, 0.0f);
	blueKeyIcon->SetSRV(blueKeyIconTex->GetSRV());

	// red key icon
	DirectX::XMFLOAT3 redKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 10, WinMaxHeight - 610, 0);
	DirectX::XMFLOAT3 redKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	Texture2D* redKeyIconTex = new Texture2D(IconsPath + L"redkey.png");
	redKeyIcon = new TextureRect(redKeyIonPosition, redKeyIonSize, 0.0f);
	redKeyIcon->SetSRV(redKeyIconTex->GetSRV());

	// green key icon
	DirectX::XMFLOAT3 greenKeyIonPosition = DirectX::XMFLOAT3(TileWidth - 10, WinMaxHeight - 660, 0);
	DirectX::XMFLOAT3 greenKeyIonSize = DirectX::XMFLOAT3(TileWidth, TileHeight, 1);
	Texture2D* greenKeyIconTex = new Texture2D(IconsPath + L"greenkey.png");
	greenKeyIcon = new TextureRect(greenKeyIonPosition, greenKeyIonSize, 0.0f);
	greenKeyIcon->SetSRV(greenKeyIconTex->GetSRV());
}
                            
void GameUI::Render()
{
	switch (mode)
	{
	case STARTMENU:
		for (int i = 0; i < startButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				startButtons[i]->text.c_str(),
				startButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				startButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		base->Render();
		startCursor.textureRect->Render();
		break;

	case INGAMEUI:
		for (int i = 0; i < gameMenuButtons.size(); i++)
		{
			mD2DResource->pD2DRenderTarget->DrawText(
				gameMenuButtons[i]->text.c_str(),
				gameMenuButtons[i]->text.size(),
				mD2DResource->pTextFormat,
				gameMenuButtons[i]->textRect,
				mD2DResource->pSolidColorBrush);
		}
		
		rightDispalyBase->Render();
		leftButtonBase->Render();
		timerBase->Render();
		walkingStepBase->Render();

		mainGameCursor_1st.textureRect->Render();
		if (UIstate == UIState::MenuLevel2) mainGameCursor_2nd.textureRect->Render();
		RenderMonsterStates();
		break;
	case PLAYERSTATES:
		RenderPlayerStates();
		break;
	}	

}

void GameUI::Update()
{
	switch (mode)
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

		// button and cursor
		UpdateCursorAndButton(mainGameCursor_1st, gameMenuButtons);

		if (UIstate == UIState::MenuLevel2)
			UpdateMonsterCursor(mainGameCursor_2nd);

		// monster Icon
		//slimeIcon->Update();

		switch (UIstate)
		{
		case UIState::MenuLevel1:
			// in game menu switch
			if (!keyboard.Press('X'))
				mApp.SetAllowSwitch(true);
			if (keyboard.Down('X') && mApp.AvailableToSwitch())
			{
				mApp.SetAllowSwitch(false);
				mApp.SetGameMode(GameMode::GAMEPLAY);
			}
			else if (keyboard.Press('C'))
			{
				UIstate = UIState::MenuLevel2;
				mainGameCursor_1st.enabled = false;
				mainGameCursor_2nd.enabled = true;
				mainGameCursor_1st.textureRect->SetCursorEnable(false);
			}
				
			break;
		case UIState::MenuLevel2:
			if (keyboard.Press('X'))
			{
				UIstate = UIState::MenuLevel1;
				mainGameCursor_1st.enabled = true;
				mainGameCursor_1st.textureRect->SetCursorEnable(true);
			}
			break;
		}

			
		break;
	case PLAYERSTATES:
		playerIcon->Update();
		yellowKeyIcon->Update();
		blueKeyIcon->Update();
		redKeyIcon->Update();
		greenKeyIcon->Update();
	}
}

void GameUI::UpdateTextRect(D2D1_RECT_F* textRect)
{
	float positionX = textRect->left / WinMaxWidth;
	float positionY = textRect->top / WinMaxHeight;
	float textWidth = textRect->right - textRect->left;
	float textHeight = textRect->bottom - textRect->top;
	textRect->left = curWinSize->width * positionX;
	textRect->top = curWinSize->height * positionY;
	textRect->right = textRect->left + textWidth;
	textRect->bottom = textRect->top + textHeight;
}

void GameUI::UpdateMonsterCursor(Cursor& cursor)
{
	curTime = Timer::TotalTime();
	if (cursor.isPressed)
	{
		// wait for key up
		if (Keyboard::get_instance().Up(VK_UP))
		{
			cursor.MoveUp();
			cursor.isPressed = false;
		}
		else if (Keyboard::get_instance().Up(VK_DOWN))
		{
			cursor.MoveDown();
			cursor.isPressed = false;
		}

		// if still pressed, move cursor
		curTime = Timer::TotalTime();
		if (curTime - lastTime > cursor.cursorMoveTimeInterval)
		{
			lastTime = curTime;
			if (cursor.moveDirection == Cursor::UP)
				cursor.MoveUp();
			else
				cursor.MoveDown();
		}
	}
	else if (Keyboard::get_instance().Press(VK_UP))
	{
		cursor.moveDirection = Cursor::UP;
		cursor.isPressed = true;
		lastTime = curTime;
	}
	else if (Keyboard::get_instance().Press(VK_DOWN))
	{
		cursor.moveDirection = Cursor::DOWN;
		cursor.isPressed = true;
		lastTime = curTime;
	}

	cursor.textureRect->Update();
}

void GameUI::UpdateCursorAndButton(Cursor& cursor, std::vector<Button*>& buttons)
{
	curTime = Timer::TotalTime();
	if (cursor.enabled)
	{
		if (cursor.isPressed)
		{
			// wait for key up
			if (Keyboard::get_instance().Up(VK_UP))
			{
				cursor.MoveUp(buttons);
				cursor.isPressed = false;
			}
			else if (Keyboard::get_instance().Up(VK_DOWN))
			{
				cursor.MoveDown(buttons);
				cursor.isPressed = false;
			}

			// if still pressed, move cursor
			curTime = Timer::TotalTime();
			if (curTime - lastTime > cursor.cursorMoveTimeInterval)
			{
				lastTime = curTime;
				if (cursor.moveDirection == Cursor::UP)
					cursor.MoveUp(buttons);
				else
					cursor.MoveDown(buttons);
			}
		}
		else if (Keyboard::get_instance().Press(VK_UP))
		{
			cursor.moveDirection = Cursor::UP;
			cursor.isPressed = true;
			lastTime = curTime;
		}
		else if (Keyboard::get_instance().Press(VK_DOWN))
		{
			cursor.moveDirection = Cursor::DOWN;
			cursor.isPressed = true;
			lastTime = curTime;
		}

		if (Keyboard::get_instance().Down('C') ||
			Keyboard::get_instance().Down(VK_RETURN) ||
			Keyboard::get_instance().Down(VK_SPACE))
		{
			cursor.Execute(buttons);
		}
	}

	cursor.textureRect->Update();

	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->UpdateTextRect(curWinSize);

	}
}

void GameUI::RenderMonsterStates()
{
	//for (auto& monsterIcon : monsterIcons)
	//	monsterIcon->Render();

	UINT position = 0;
	for (auto& monster : monsters)
	{
		if (position == 7)
			break;
		if (std::get<0>(monster.second) == position)
		{
			std::get<1>(monster.second)->Render();
			UINT spacing = 100;
			MonsterData data = std::get<2>(monster.second);
			std::wstring name = data.name;
			D2D1_RECT_F nameTextRect = D2D1::RectF(400, 30 + position * spacing, 500, 63 + position * spacing);
			UpdateTextRect(&nameTextRect);
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


			//Reset transformation matrix
			mD2DResource->pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			std::wstring attribute = L"普通";
			D2D1_RECT_F attributeTextRect = D2D1::RectF(400, 63 + position * spacing, 500, 98 + position * spacing);
			UpdateTextRect(&attributeTextRect);
			textFormat = DynamicTextFormat(attribute, &attributeTextRect);
			DrawTextWithSpacing(textFormat, attribute, &attributeTextRect);

			std::wstring HP = L"生命";
			D2D1_RECT_F HPTextRect = D2D1::RectF(505, 25 + position * spacing, 560, 60 + position * spacing);
			UpdateTextRect(&HPTextRect);
			textFormat = DynamicTextFormat(HP, &HPTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				HP.c_str(),
				HP.length(),
				textFormat,
				HPTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v1 = std::to_wstring(data.HP);
			D2D1_RECT_F v1TextRect = D2D1::RectF(570, 25 + position * spacing, 640, 60 + position * spacing);
			UpdateTextRect(&v1TextRect);
			textFormat = DynamicTextFormat(v1, &v1TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v1.c_str(),
				v1.length(),
				textFormat,
				v1TextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring attack = L"攻撃";
			D2D1_RECT_F attackTextRect = D2D1::RectF(650, 25 + position * spacing, 705, 60 + position * spacing);
			UpdateTextRect(&attackTextRect);
			textFormat = DynamicTextFormat(attack, &attackTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				attack.c_str(),
				attack.length(),
				textFormat,
				attackTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v2 = std::to_wstring(data.attack);
			D2D1_RECT_F v2TextRect = D2D1::RectF(715, 25 + position * spacing, 785, 60 + position * spacing);
			UpdateTextRect(&v2TextRect);
			textFormat = DynamicTextFormat(v2, &v2TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v2.c_str(),
				v2.length(),
				textFormat,
				v2TextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring defense = L"防御";
			D2D1_RECT_F defenseTextRect = D2D1::RectF(795, 25 + position * spacing, 850, 60 + position * spacing);
			UpdateTextRect(&defenseTextRect);
			textFormat = DynamicTextFormat(defense, &defenseTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				defense.c_str(),
				defense.length(),
				textFormat,
				defenseTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v3 = std::to_wstring(data.defense);
			D2D1_RECT_F v3TextRect = D2D1::RectF(860, 25 + position * spacing, 930, 60 + position * spacing);
			UpdateTextRect(&v3TextRect);
			textFormat = DynamicTextFormat(v3, &v3TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v3.c_str(),
				v3.length(),
				textFormat,
				v3TextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring gold = L"GOLD";
			D2D1_RECT_F goldTextRect = D2D1::RectF(505, 65 + position * spacing, 560, 100 + position * spacing);
			UpdateTextRect(&goldTextRect);
			textFormat = DynamicTextFormat(HP, &HPTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				gold.c_str(),
				gold.length(),
				textFormat,
				goldTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v4 = std::to_wstring(data.gold);
			D2D1_RECT_F v4TextRect = D2D1::RectF(570, 65 + position * spacing, 640, 100 + position * spacing);
			UpdateTextRect(&v4TextRect);
			textFormat = DynamicTextFormat(v4, &v4TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v4.c_str(),
				v4.length(),
				textFormat,
				v4TextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring exp = L"EXP";
			D2D1_RECT_F expTextRect = D2D1::RectF(650, 65 + position * spacing, 705, 100 + position * spacing);
			UpdateTextRect(&expTextRect);
			textFormat = DynamicTextFormat(exp, &expTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				exp.c_str(),
				exp.length(),
				textFormat,
				expTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v5 = std::to_wstring(data.exp);
			D2D1_RECT_F v5TextRect = D2D1::RectF(715, 65 + position * spacing, 785, 100 + position * spacing);
			UpdateTextRect(&v5TextRect);
			textFormat = DynamicTextFormat(v5, &v5TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v5.c_str(),
				v5.length(),
				textFormat,
				v5TextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring damage = L"ダメージ";
			D2D1_RECT_F damageTextRect = D2D1::RectF(795, 65 + position * spacing, 850, 100 + position * spacing);
			UpdateTextRect(&damageTextRect);
			textFormat = DynamicTextFormat(damage, &damageTextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				damage.c_str(),
				damage.length(),
				textFormat,
				damageTextRect,
				mD2DResource->pSolidColorBrush);

			std::wstring v6 = L"10000";
			D2D1_RECT_F v6TextRect = D2D1::RectF(860, 65 + position * spacing, 930, 100 + position * spacing);
			UpdateTextRect(&v6TextRect);
			textFormat = DynamicTextFormat(v6, &v6TextRect);
			mD2DResource->pD2DRenderTarget->DrawText(
				v6.c_str(),
				v6.length(),
				textFormat,
				v6TextRect,
				mD2DResource->pSolidColorBrush);

			position++;
		}

	}
}

void GameUI::RenderPlayerStates()
{
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
		D2D1::RectF(180, 40, 180, 40),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"レベル"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 95, 150, 95),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"生命力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 150, 150, 150),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"攻撃力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 205, 150, 205),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"防御力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 260, 150, 260),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"魔防力"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 315, 150, 315),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"EXP"),
		4,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 370, 150, 370),
		mD2DResource->pSolidColorBrush);
	mD2DResource->pD2DRenderTarget->DrawText(
		(L"GOLD"),
		5,
		mD2DResource->pTextFormat,
		D2D1::RectF(40, 425, 150, 425),
		mD2DResource->pSolidColorBrush);

	playerIcon->Render();
	yellowKeyIcon->Render();
	blueKeyIcon->Render();
	redKeyIcon->Render();
	greenKeyIcon->Render();
}

IDWriteTextFormat* GameUI::DynamicTextFormat(const std::wstring& text, const D2D1_RECT_F* textRect, bool* isTransformed)
{
	IDWriteTextFormat* textFormat = nullptr;
	float fontSize = 30.0f;
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