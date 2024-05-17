#pragma once

#include "Util.h"
#include "SingletonBase.h"

#define MAX_INPUT_MOUSE 8

class Mouse : public SingletonBase<Mouse>
{
public:
	friend class SingletonBase<Mouse>;

	void SetHandle(HWND handle)
	{
		this->handle = handle;
	}

	void Update();

	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	DirectX::XMFLOAT3 GetPosition() { return position; }

	bool Down(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;
	}

	bool Up(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_UP;
	}

	bool Press(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;
	}

	DirectX::XMFLOAT3 GetMoveValue()
	{
		return wheelMoveValue;
	}

private:
	Mouse();
	~Mouse();

	HWND handle;
	DirectX::XMFLOAT3 position; //마퓖E?위치

	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	DirectX::XMFLOAT3 wheelStatus;
	DirectX::XMFLOAT3 wheelOldStatus;
	DirectX::XMFLOAT3 wheelMoveValue;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	enum
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};
};

