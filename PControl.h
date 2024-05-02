#pragma once

#include "Animator.h"
#include "Keyboard.h"

static int* checker;



class PControl
{
public:
	PControl();
	~PControl();

	void Update();

	void Idle();

	// character control
	void Right(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName);
	void Left(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName);
	void Up(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName);
	void Down(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName);


	int GetFacingWhere() { return FacingWhere; }
	void SetFacingWhere(int i) { FacingWhere = i; }


	void Attack(DWORD key);
	void Skill(DWORD key, DWORD key2);


	// enemy control
	void move(DirectX::XMFLOAT3* position, DirectX::XMFLOAT3* targetPosition, float speed, std::wstring clipName1, std::wstring clipName2);




	// arrow control
	//void ArrowUp(DWORD key, Vector3* position, float speed);
	void ArrowRight(DWORD key, DirectX::XMFLOAT3* position, float speed, int _FacingWhere);



	//normal control ( Ãæµ¹ ÈÄ ¹ÝÀÀÀ» À§ÇØ »ç¿EÒ ÇÔ¼E
	void collision(DirectX::XMFLOAT3* position, float speedx, float speedy);

	void SetAnimator(Animator** animator) { this->animator = animator; }
	int GetSkill() { return _Skill; }



private:
	class Animator** animator = nullptr;

	Keyboard& keyboard = Keyboard::get_instance();


	bool FacingLeft = false;
	bool FacingUp = false;
	bool UDCheck = false;
	bool jump = false;
	bool attack = false;


	int _Skill = 4; //0 is left, 1 is right, 2 is Up, 3 is Down, 4 is notshooting
	int tmp_Face = 0;

	int FacingWhere = 1; //0 is Left , 1 is Right , 2 is Up, 3 is Down 


	float deltaTime = 0.0f;
	float keyDown = 0.0f;
	float checker = 0.0f;
};
