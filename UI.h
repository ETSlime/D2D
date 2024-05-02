#pragma once

#include "UIRect.h"
#include "UIalphabet.h"
#include "Keyboard.h"
#include "Mouse.h"

class UI
{
public:
	/*
	mode 0 ﾀｺ ｰﾔﾀﾓｽﾃﾀﾛ 1 ﾀｺ ui ｽﾃｰ｣ｰ・ﾃﾇﾔｼ・2 ｴﾂ pauseｰ・ﾃ 3ﾀｺ ｷｹｺｧｾ・ｰ・ﾃ 4ｴﾂ ｽﾂｸｮ ｹﾗ ﾆﾐｹ隹・ﾃ 5ｴﾂ ｰﾔﾀﾓｳｻｺﾎ ui
	*/
	UI(ID3D11Device* device,ID3D11DeviceContext* deviceContext, 
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, int mode);
	~UI();

	void init_startUI();
	void init_pauseUI();
	void init_levelupUI();
	void init_time();
	void init_result();
	void init_gameUI();
	void init_NumstartUI();



	void Update();
	void Render();
	UIRect* GetanimRect() { return animRect; }

	//for helloworld
	void UpdateStart();
	void RenderStart();

	//for levelup

	void UpdateLevelup();
	void RenderLevelup();

	//for Time UI
	void UpdateTime(int _time);
	void RenderTime();


	//result UI
	void UpdateWin();
	void UpdateLose();
	void RenderWin();
	void RenderLose();

	//for Game UI
	void UpdategameUI(int LVcount, int ATcount, int AScount);
	void RendergameUI();

	//for NumStartUi

	void UpdateNumstartUI(int _time);
	void RenderNumstartUI();


	int GetPressWhat() { return pressWhat; }
	void SetPressWhat(int a) { this->pressWhat = a; }



private:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	int mode = 0;  // mode 0 ﾀｺ ｰﾔﾀﾓｽﾃﾀﾛ 1 ﾀｺ ui ｽﾃｰ｣ｰ・ﾃﾇﾔｼ・2 ｴﾂ pauseｰ・ﾃ 3ﾀｺ ｷｹｺｧｾ・ｰ・ﾃ 4ｴﾂ ｽﾂｸｮ ｹﾗ ﾆﾐｹ隹・ﾃ


	//ｽﾃｰ｣ ｰ・ﾃ 
	class UIRect* Time1 = nullptr;
	class UIRect* Time2 = nullptr;
	class UIRect* Time3 = nullptr;
	class UIRect* Time4 = nullptr;

	//ｷｹｺｧｾ・ﾃ｢ ｰ・ﾃ
	class TextureRect* Levelup = nullptr;

	class UITextureRect* attackup = nullptr;
	class UITextureRect* HPup = nullptr;
	class UITextureRect* attackspeedup = nullptr;




	//ｰﾔﾀﾓ ｽﾃﾀﾛ ｰ・ﾃ
	class UITextureRect* start = nullptr;
	class UITextureRect* how = nullptr;
	class TextureRect* howto = nullptr;

	//ｽﾂｸｮ ﾆﾐｹ・UI ｰ・ﾃ
	class TextureRect* winUI = nullptr;
	class TextureRect* LoseUI = nullptr;

	//ｰﾔﾀﾓUI ｰ・ﾃ
	class TextureRect* gameUI = nullptr;
	class UIRect* levelupcount = nullptr;
	class UIRect* levelupcount2 = nullptr;
	class UIRect* attackcount = nullptr;
	class UIRect* attackcount2 = nullptr;
	class UIRect* attackspeedcount = nullptr;
	class UIRect* attackspeedcount2 = nullptr;



	UIalphabet* gameUIalphabet1 = nullptr;
	UIalphabet* gameUIalphabet2 = nullptr;
	UIalphabet* gameUIalphabet3 = nullptr;
	UIalphabet* gameUIalphabet4 = nullptr;
	UIalphabet* gameUIalphabet5 = nullptr;
	UIalphabet* gameUIalphabet6 = nullptr;






	UIRect* animRect = nullptr;
	UIalphabet* Timealphabet1 = nullptr;
	UIalphabet* Timealphabet2 = nullptr;
	UIalphabet* Timealphabet3 = nullptr;
	UIalphabet* Timealphabet4 = nullptr;

	int pressWhat = 0;
	/*
	ｰ｢ｰ｢ﾀﾇ UI ｰ｡ ｹｫｾｻ ｹﾞｾﾆｵ鯀ｴｴﾂﾁ・ｸﾞｽﾃﾁｦ ｺｸｳｻｴﾂｵ･ ｾｲｴﾂ ｺｯｼ・ｰ｢ｰ｢ ｴﾙｸ･ｿ・ｵｷﾎ ｾｲﾀﾎｴﾙ

	time UI : nothing (ｾﾈｾｲﾀﾓ)

	startUI : demoﾆﾄﾀﾏｿ｡ ｴﾙﾀｽﾀｸｷﾎ ｳﾑﾁ愠・ｾﾈｳﾑｱ戝｡ ｴ・ﾘ ｻ鄙・ﾏｴﾂ ｿ・ｵ (1: ｳﾑｱ・0: ｾﾈｳﾑｱ・

	levelupUI : ｰﾔﾀﾓｿ｡ｰﾔ ﾀﾌｻ邯ﾌ ｷｹｺｧｾｸｷﾎ ｹｫｽｼ ｴﾉｷﾂﾄ｡ｸｦ ｻ鄙・ﾏｿｴｴﾂﾁ・ｾﾋｷﾁﾁﾖｴﾂ ｺｯｼ・0: ｱﾗｴ・ﾎ , 1 : hpup 2: attackup 3: attackspeedUP
	*/


	bool howtoUI = false;
	int time = 0;

	Keyboard& keyboard = Keyboard::get_instance();
	Mouse& mouse = Mouse::get_instance();
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};