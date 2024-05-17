#include "UI.h"
#include "UIRect.h"
#include "TextureRect.h"
#include "UITextureRect.h"
#include "Keyboard.h"
#include "Timer.h"

UI::UI(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, int mode)
	:mDevice(device), mDeviceContext(deviceContext), position(position), size(size), mode(mode)
{
	if (mode == 0)
		init_startUI();
	else if (mode == 1)
		init_time();
	else if (mode == 2)
		init_pauseUI();
	else if (mode == 3)
		init_levelupUI();
	else if (mode == 4)
		init_result();
	else if (mode == 5)
		init_gameUI();
	else if (mode == 6)
		init_NumstartUI();

}

UI::~UI()
{


}

void UI::init_startUI()
{
	//start = new UITextureRect(mDevice, mDeviceContext, position, size, 0.0f, TexturePath + L"GameStart.png");
	//how = new UITextureRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3(0, (1.3) * size.y, 0), size, 0.0f, TexturePath + L"howtoplay.png");

	howto = new TextureRect(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 360, 0), DirectX::XMFLOAT3(800, 600, 1), 0.0f, TexturePath + L"howto.png");


}

void UI::init_pauseUI()
{
}

void UI::init_levelupUI()
{
	Levelup = new TextureRect(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 360, 0), DirectX::XMFLOAT3(800, 600, 1), 0.0f, TexturePath + L"LEVELUP.png");

	/*HPup = new UITextureRect(mDevice, mDeviceContext, position, size, 0.0f, TexturePath + L"healing.png");
	attackup = new UITextureRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3(0, (1.3) * size.y, 0), size, 0.0f, TexturePath + L"attackincrease.png");
	attackspeedup = new UITextureRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3(0, (2.6) * size.y, 0), size, 0.0f, TexturePath + L"attackspeed.png");*/
}

void UI::init_time()
{

	Time1 = new UIRect(mDevice, mDeviceContext, position, size);
	Time2 = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3(size.x, 0, 0), size);
	Time3 = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3(3 * size.x, 0, 0), size);
	Time4 = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3(4 * size.x, 0, 0), size);
	Timealphabet1 = new UIalphabet();


	Texture2D* srcTex = new Texture2D(mDevice, mDeviceContext, TexturePath + L"number.png"); // 이미햨E소스

	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(srcTex->GetWidth(), srcTex->GetHeight());



	/*

	Time UI




	*/


	// idle Anim
	UIClip* Num1 = new UIClip(L"Num1", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.33f));
	UIClip* Num2 = new UIClip(L"Num2", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f));
	UIClip* Num3 = new UIClip(L"Num3", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f));
	UIClip* Num4 = new UIClip(L"Num4", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f));
	UIClip* Num5 = new UIClip(L"Num5", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, 0.0f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.167f));
	UIClip* Num6 = new UIClip(L"Num6", srcTex, DirectX::XMFLOAT2(0.0f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.33f));
	UIClip* Num7 = new UIClip(L"Num7", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.33f));
	UIClip* Num8 = new UIClip(L"Num8", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.33f));
	UIClip* Num9 = new UIClip(L"Num9", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.33f));
	UIClip* Num0 = new UIClip(L"Num0", srcTex, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f));

	//clip save
	Timealphabet1->SetUI(Num1);
	Timealphabet1->SetUI(Num2);
	Timealphabet1->SetUI(Num3);
	Timealphabet1->SetUI(Num4);
	Timealphabet1->SetUI(Num5);
	Timealphabet1->SetUI(Num6);
	Timealphabet1->SetUI(Num7);
	Timealphabet1->SetUI(Num8);
	Timealphabet1->SetUI(Num9);
	Timealphabet1->SetUI(Num0);





	Timealphabet1->SetCurrentUIClip(L"Num0");


	Timealphabet2 = new UIalphabet(*Timealphabet1);
	Timealphabet3 = new UIalphabet(*Timealphabet1);
	Timealphabet4 = new UIalphabet(*Timealphabet1);



	Time1->SetAnimation(Timealphabet1);
	Time2->SetAnimation(Timealphabet2);
	Time3->SetAnimation(Timealphabet3);
	Time4->SetAnimation(Timealphabet4);








	SAFE_DELETE(srcTex);
}

void UI::init_result()
{
	winUI = new TextureRect(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 0, 0), DirectX::XMFLOAT3(400, 100, 1), 0.0f, TexturePath + L"WINUI.png");
	LoseUI = new TextureRect(mDevice, mDeviceContext, DirectX::XMFLOAT3(640, 0, 0), DirectX::XMFLOAT3(400, 100, 1), 0.0f, TexturePath + L"YOULOSE.png");
}

void UI::init_gameUI()
{

	gameUI = new TextureRect(mDevice, mDeviceContext, position, DirectX::XMFLOAT3(300, 100, 1), 0.0f, TexturePath + L"gameUI.png");

	levelupcount = new UIRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3((3.5) * 30, 15, 0), DirectX::XMFLOAT3(30, 60, 1));
	levelupcount2 = new UIRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3((2.5) * 30, 15, 0), DirectX::XMFLOAT3(30, 60, 1));

	attackcount = new UIRect(mDevice, mDeviceContext, position - DirectX::XMFLOAT3(15, 15, 0), DirectX::XMFLOAT3(30, 60, 1));
	attackcount2 = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3(15, -15, 0), DirectX::XMFLOAT3(30, 60, 1));

	attackspeedcount = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3((2.5) * 30, -15, 0), DirectX::XMFLOAT3(30, 60, 1));
	attackspeedcount2 = new UIRect(mDevice, mDeviceContext, position + DirectX::XMFLOAT3((3.5) * 30, -15, 0), DirectX::XMFLOAT3(30, 60, 1));

	Timealphabet1 = new UIalphabet();


	Texture2D* srcTex = new Texture2D(mDevice, mDeviceContext, TexturePath + L"number.png"); // 이미햨E소스

	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(srcTex->GetWidth(), srcTex->GetHeight());



	gameUIalphabet1 = new UIalphabet();




	// idle Anim
	UIClip* Num1 = new UIClip(L"Num1", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.33f));
	UIClip* Num2 = new UIClip(L"Num2", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f));
	UIClip* Num3 = new UIClip(L"Num3", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f));
	UIClip* Num4 = new UIClip(L"Num4", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f));
	UIClip* Num5 = new UIClip(L"Num5", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, 0.0f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.167f));
	UIClip* Num6 = new UIClip(L"Num6", srcTex, DirectX::XMFLOAT2(0.0f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.33f));
	UIClip* Num7 = new UIClip(L"Num7", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.33f));
	UIClip* Num8 = new UIClip(L"Num8", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.33f));
	UIClip* Num9 = new UIClip(L"Num9", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.33f));
	UIClip* Num0 = new UIClip(L"Num0", srcTex, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f));

	//clip save
	gameUIalphabet1->SetUI(Num1);
	gameUIalphabet1->SetUI(Num2);
	gameUIalphabet1->SetUI(Num3);
	gameUIalphabet1->SetUI(Num4);
	gameUIalphabet1->SetUI(Num5);
	gameUIalphabet1->SetUI(Num6);
	gameUIalphabet1->SetUI(Num7);
	gameUIalphabet1->SetUI(Num8);
	gameUIalphabet1->SetUI(Num9);
	gameUIalphabet1->SetUI(Num0);





	gameUIalphabet1->SetCurrentUIClip(L"Num0");



	gameUIalphabet2 = new UIalphabet(*gameUIalphabet1);
	gameUIalphabet3 = new UIalphabet(*gameUIalphabet1);
	gameUIalphabet4 = new UIalphabet(*gameUIalphabet1);
	gameUIalphabet5 = new UIalphabet(*gameUIalphabet1);
	gameUIalphabet6 = new UIalphabet(*gameUIalphabet1);


	levelupcount->SetAnimation(gameUIalphabet1);
	levelupcount2->SetAnimation(gameUIalphabet2);
	attackcount->SetAnimation(gameUIalphabet3);
	attackcount2->SetAnimation(gameUIalphabet4);
	attackspeedcount->SetAnimation(gameUIalphabet5);
	attackspeedcount2->SetAnimation(gameUIalphabet6);










	SAFE_DELETE(srcTex);
}

void UI::init_NumstartUI()
{
	Time1 = new UIRect(mDevice, mDeviceContext, position, size);


	Timealphabet1 = new UIalphabet();


	Texture2D* srcTex = new Texture2D(mDevice, mDeviceContext, TexturePath + L"number.png"); // 이미햨E소스

	DirectX::XMFLOAT2 texSize = DirectX::XMFLOAT2(srcTex->GetWidth(), srcTex->GetHeight());

	UIClip* Num1 = new UIClip(L"Num1", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.33f));
	UIClip* Num2 = new UIClip(L"Num2", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f));
	UIClip* Num3 = new UIClip(L"Num3", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f));
	UIClip* Num4 = new UIClip(L"Num4", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.167f));
	UIClip* Num5 = new UIClip(L"Num5", srcTex, DirectX::XMFLOAT2(texSize.x * 0.8f, 0.0f), DirectX::XMFLOAT2(texSize.x, texSize.y * 0.167f));
	UIClip* Num6 = new UIClip(L"Num6", srcTex, DirectX::XMFLOAT2(0.0f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.33f));
	UIClip* Num7 = new UIClip(L"Num7", srcTex, DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.33f));
	UIClip* Num8 = new UIClip(L"Num8", srcTex, DirectX::XMFLOAT2(texSize.x * 0.4f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.33f));
	UIClip* Num9 = new UIClip(L"Num9", srcTex, DirectX::XMFLOAT2(texSize.x * 0.6f, texSize.y * 0.167f), DirectX::XMFLOAT2(texSize.x * 0.8f, texSize.y * 0.33f));
	UIClip* Num0 = new UIClip(L"Num0", srcTex, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(texSize.x * 0.2f, texSize.y * 0.167f));

	Timealphabet1->SetUI(Num1);
	Timealphabet1->SetUI(Num2);
	Timealphabet1->SetUI(Num3);
	Timealphabet1->SetUI(Num4);
	Timealphabet1->SetUI(Num5);
	Timealphabet1->SetUI(Num6);
	Timealphabet1->SetUI(Num7);
	Timealphabet1->SetUI(Num8);
	Timealphabet1->SetUI(Num9);
	Timealphabet1->SetUI(Num0);


	Timealphabet1->SetCurrentUIClip(L"Num1");


	Time1->SetAnimation(Timealphabet1);
}

void UI::Update()
{




}

void UI::Render()
{




	animRect->Render();

}

void UI::UpdateStart()
{
	DirectX::XMFLOAT3 mpos = mouse.GetPosition();
	//pressWhat = 0;


	start->Update();
	how->Update();





	if (howtoUI == true)
	{
		howto->Update();
		if (keyboard.Down('Q'))
			howtoUI = false;
	}


	if (howtoUI == false)
	{
		DirectX::XMFLOAT3 mpos = mouse.GetPosition();
		if (mpos.x < ((float)position.x + (float)(0.5) * size.x) &&
			mpos.x >((float)position.x - (float)(0.5) * size.x) &&
			mpos.y < ((float)position.y + (float)(0.5) * size.y) &&
			mpos.y >((float)position.y - (float)(0.5) * size.y)
			)
		{
			start->changeshaded(1);
			if (mouse.Press(0))
				pressWhat = 1;

		}
		else
		{
			start->changeshaded(0);

		}


		if (mpos.x < ((float)position.x + (float)(0.5) * size.x) &&
			mpos.x >((float)position.x - (float)(0.5) * size.x) &&
			mpos.y < ((float)(position.y - (1.3) * size.y) + (float)(0.5) * size.y) &&
			mpos.y >((float)(position.y - (1.3) * size.y) - (float)(0.5) * size.y)
			)
		{
			how->changeshaded(1);

			if (mouse.Press(0))
			{
				howtoUI = true;
			}
		}
		else
		{
			how->changeshaded(0);

		}
	}




}

void UI::RenderStart()
{



	start->Render();
	how->Render();

	if (howtoUI == true)
	{
		howto->Render();

	}




}

void UI::UpdateLevelup()
{
	DirectX::XMFLOAT3 mpos = mouse.GetPosition();

	if (mpos.x < ((float)position.x + (float)(0.5) * size.x) &&
		mpos.x >((float)position.x - (float)(0.5) * size.x) &&
		mpos.y < ((float)position.y + (float)(0.5) * size.y) &&
		mpos.y >((float)position.y - (float)(0.5) * size.y)
		)
	{
		HPup->changeshaded(1);
		if (mouse.Press(0))
			pressWhat = 1;

	}
	else
	{
		HPup->changeshaded(0);

	}

	if (mpos.x < ((float)position.x + (float)(0.5) * size.x) &&
		mpos.x >((float)position.x - (float)(0.5) * size.x) &&
		mpos.y < ((float)(position.y - (1.3) * size.y) + (float)(0.5) * size.y) &&
		mpos.y >((float)(position.y - (1.3) * size.y) - (float)(0.5) * size.y)
		)
	{
		attackup->changeshaded(1);
		if (mouse.Press(0))
			pressWhat = 2;


	}
	else
	{
		attackup->changeshaded(0);

	}

	if (mpos.x < ((float)position.x + (float)(0.5) * size.x) &&
		mpos.x >((float)position.x - (float)(0.5) * size.x) &&
		mpos.y < ((float)position.y - (2.6) * size.y + (float)(0.5) * size.y) &&
		mpos.y >((float)position.y - (2.6) * size.y - (float)(0.5) * size.y)
		)
	{
		attackspeedup->changeshaded(1);
		if (mouse.Press(0))
			pressWhat = 3;

	}
	else
	{
		attackspeedup->changeshaded(0);

	}


	if (pressWhat == 0)
	{
		Levelup->Update();
		HPup->Update();
		attackup->Update();
		attackspeedup->Update();
	}
}

void UI::RenderLevelup()
{
	if (pressWhat == 0)
	{
		Levelup->Render();
		HPup->Render();
		attackup->Render();
		attackspeedup->Render();
	}
}

void UI::UpdateTime(int _time)
{
	/*들엉邦는 시간을 분초에 맞춰서 변경후, 클립으로 바꾸는 과정*/

	time = _time;
	int tmptime = time % 10;
	std::wstring num4 = L"Num";
	num4 += std::to_wstring(tmptime);
	time -= tmptime;
	tmptime = time % 60;
	time -= tmptime;
	tmptime /= 10;
	std::wstring num3 = L"Num";
	num3 += std::to_wstring(tmptime);
	time /= 60;
	tmptime = time % 10;
	std::wstring num2 = L"Num";
	num2 += std::to_wstring(tmptime);
	time /= 10;
	tmptime = time % 10;
	std::wstring num1 = L"Num";
	num1 += std::to_wstring(tmptime);


	Timealphabet4->SetCurrentUIClip(num4);
	Timealphabet3->SetCurrentUIClip(num3);
	Timealphabet2->SetCurrentUIClip(num2);
	Timealphabet1->SetCurrentUIClip(num1);


	Timealphabet1->Update();
	Timealphabet2->Update();
	Timealphabet3->Update();
	Timealphabet4->Update();
	Time1->Update();
	Time2->Update();
	Time3->Update();
	Time4->Update();
}



void UI::RenderTime()
{
	Time1->Render();
	Time2->Render();
	Time3->Render();
	Time4->Render();
}

void UI::UpdateWin()
{
	winUI->Update();
	//if ((*winUI->Getpos()).y < 400)
	//	(*winUI->Getpos()).y += Timer::Delta() * 200;

}

void UI::UpdateLose()
{
	LoseUI->Update();
	//if ((*LoseUI->Getpos()).y < 400)
	//	(*LoseUI->Getpos()).y += Timer::Delta() * 200;
}

void UI::RenderWin()
{
	winUI->Render();
}

void UI::RenderLose()
{
	LoseUI->Render();
}

void UI::UpdategameUI(int LVcount, int ATcount, int AScount)
{
	gameUI->Update();


	int tmp_data = LVcount % 10;
	std::wstring num2 = L"Num";
	num2 += std::to_wstring(tmp_data);
	tmp_data = (LVcount - LVcount % 10) / 10;
	tmp_data = tmp_data % 10;
	std::wstring num1 = L"Num";
	num1 += std::to_wstring(tmp_data);


	gameUIalphabet2->SetCurrentUIClip(num2);
	gameUIalphabet1->SetCurrentUIClip(num1);
	gameUIalphabet1->Update();
	gameUIalphabet2->Update();
	levelupcount->Update();
	levelupcount2->Update();



	tmp_data = ATcount % 10;
	num2 = L"Num";
	num2 += std::to_wstring(tmp_data);
	tmp_data = (ATcount - ATcount % 10) / 10;
	tmp_data = tmp_data % 10;
	num1 = L"Num";
	num1 += std::to_wstring(tmp_data);


	gameUIalphabet4->SetCurrentUIClip(num2);
	gameUIalphabet3->SetCurrentUIClip(num1);
	gameUIalphabet3->Update();
	gameUIalphabet4->Update();
	attackcount->Update();
	attackcount2->Update();


	tmp_data = AScount % 10;
	num2 = L"Num";
	num2 += std::to_wstring(tmp_data);
	tmp_data = (AScount - AScount % 10) / 10;
	tmp_data = tmp_data % 10;
	num1 = L"Num";
	num1 += std::to_wstring(tmp_data);


	gameUIalphabet6->SetCurrentUIClip(num2);
	gameUIalphabet5->SetCurrentUIClip(num1);
	gameUIalphabet5->Update();
	gameUIalphabet6->Update();
	attackspeedcount->Update();
	attackspeedcount2->Update();





}

void UI::RendergameUI()
{
	gameUI->Render();
	levelupcount->Render();
	levelupcount2->Render();
	attackcount->Render();
	attackcount2->Render();
	attackspeedcount->Render();
	attackspeedcount2->Render();

}

void UI::UpdateNumstartUI(int _time)
{
	int tmp = 3 - _time;
	std::wstring num = L"Num";
	num += std::to_wstring(tmp);

	Timealphabet1->SetCurrentUIClip(num);
	Timealphabet1->Update();
	Time1->Update();
}

void UI::RenderNumstartUI()
{
	Time1->Render();
}


