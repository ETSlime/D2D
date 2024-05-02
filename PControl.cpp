#include "Util.h"
#include "PControl.h"
#include "Timer.h"
#include "Keyboard.h"

PControl::PControl()
{

}

PControl::~PControl()
{
}

void PControl::Update()
{
	deltaTime += Timer::Delta();

}



/*

IDLE �Լ�E

*/


void PControl::Idle()
{
	if (FacingWhere == 0)
	{
		(*animator)->SetCurrentAnimClip(L"IdleL");
	}
	else if (FacingWhere == 1)
	{
		(*animator)->SetCurrentAnimClip(L"IdleR");
	}
	else if (FacingWhere == 2)
	{
		(*animator)->SetCurrentAnimClip(L"IdleU");
	}
	else if (FacingWhere == 3)
	{
		(*animator)->SetCurrentAnimClip(L"IdleD");
	}
}


/*

�÷��̾�ցEĳ������ �Լ�E


*/




void PControl::Right(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName)
{

	if (keyboard.Press(key) && attack == 0)
	{

		FacingLeft = false;

		if ((*position).x < 1100)
			(*position).x += speed * Timer::Delta();
		if (UDCheck == false)
		{

			FacingWhere = 1;
			(*animator)->SetCurrentAnimClip(clipName);
		}

	}
	else if (keyboard.Up(key))
		Idle();
}

void PControl::Left(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName)
{
	if (keyboard.Press(key) && attack == 0)
	{
		FacingLeft = true;

		if ((*position).x > 100)
			(*position).x -= speed * Timer::Delta();
		if (UDCheck == false)
		{
			(*animator)->SetCurrentAnimClip(clipName);
			SetFacingWhere(0);
			FacingWhere = 0;
		}



	}
	else if (keyboard.Up(key))
		Idle();
}

void PControl::Up(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName)
{
	if (keyboard.Press(key) && attack == 0)
	{
		UDCheck = true;
		FacingWhere = 2;
		(*position).y += speed * Timer::Delta();
		(*animator)->SetCurrentAnimClip(clipName);
	}
	else if (keyboard.Up(key))
	{
		Idle();
		UDCheck = false;

	}

}

void PControl::Down(DWORD key, DirectX::XMFLOAT3* position, float speed, std::wstring clipName)
{
	if (keyboard.Press(key) && attack == 0)
	{
		UDCheck = true;
		FacingWhere = 3;
		(*position).y -= speed * Timer::Delta();
		(*animator)->SetCurrentAnimClip(clipName);
	}
	else if (keyboard.Up(key))
	{
		Idle();
		UDCheck = false;

	}
}


void PControl::Attack(DWORD key)
{
	if (keyboard.Press(key))
	{
		attack = true;
		if (FacingWhere == 0)
			(*animator)->SetCurrentAnimClip(L"AttackL");
		else if (FacingWhere == 1)
			(*animator)->SetCurrentAnimClip(L"AttackR");
		else if (FacingWhere == 2)
			(*animator)->SetCurrentAnimClip(L"AttackU");
		else if (FacingWhere == 3)
			(*animator)->SetCurrentAnimClip(L"AttackD");

	}
	else if (keyboard.Up(key))
	{
		Idle();
		attack = false;
	}


}

void PControl::Skill(DWORD key, DWORD key2)
{

	if (keyboard.Press(key))
	{
		attack = true;
		if (FacingWhere == 0)
		{
			(*animator)->SetCurrentAnimClip(L"IdleL");

		}
		else if (FacingWhere == 1)
		{
			(*animator)->SetCurrentAnimClip(L"IdleR");
		}
		else if (FacingWhere == 2)
		{
			(*animator)->SetCurrentAnimClip(L"IdleU");
		}
		else if (FacingWhere == 3)
		{
			(*animator)->SetCurrentAnimClip(L"IdleD");
		}
	}
	else if (keyboard.Up(key))
	{
		Idle();
		attack = false;
	}
}





/*
���⼭���� ������ �ൿ �Լ�E


*/



void PControl::move(DirectX::XMFLOAT3* position, DirectX::XMFLOAT3* targetPosition, float speed, std::wstring clipName1, std::wstring clipName2)
{
	(*position).x += (((*targetPosition).x - (*position).x) / 2) * 1 * Timer::Delta();
	(*position).y += (((*targetPosition).y - (*position).y) / 2) * 1 * Timer::Delta();

	if (((*targetPosition).x - (*position).x) > 0)
	{
		(*animator)->SetCurrentAnimClip(clipName1);
	}
	else
	{
		(*animator)->SetCurrentAnimClip(clipName2);
	}



}




/*

���⼭���� ����ü�� �ൿ�Լ�E




*/



void PControl::ArrowRight(DWORD key, DirectX::XMFLOAT3* position, float speed, int _FacingWhere)
{



	tmp_Face = _FacingWhere;
	{
		if (tmp_Face == 0)
		{
			(*animator)->SetCurrentAnimClip(L"IdleL");
			(*position).x -= speed * Timer::Delta();
		}
		else if (tmp_Face == 1)
		{
			(*position).x += speed * Timer::Delta();
			(*animator)->SetCurrentAnimClip(L"IdleR");
		}
		else if (tmp_Face == 2)
		{
			(*position).y += speed * Timer::Delta();
			(*animator)->SetCurrentAnimClip(L"IdleU");
		}
		else if (tmp_Face == 3)
		{
			(*position).y -= speed * Timer::Delta();
			(*animator)->SetCurrentAnimClip(L"IdleD");
		}

	}

}

void PControl::collision(DirectX::XMFLOAT3* position, float speedx, float speedy)
{
	(*position).y += speedy * Timer::Delta();
	(*position).x += speedx * Timer::Delta();
}


