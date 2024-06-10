#include "Keyboard.h"


void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));


	if (GetKeyboardState(keyState))
	{
		for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
		{
			byte key = keyState[i] & 0x80;
			keyState[i] = key ? 1 : 0;

			int oldState = keyOldState[i];
			int state = keyState[i];

			if (oldState == 0 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_DOWN;
				change = true;
			}
			else if (oldState == 1 && state == 0)
			{
				keyMap[i] = KEY_INPUT_STATUS_UP;
				change = true;
			}
			else if (oldState == 1 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_PRESS;
				change = false;
			}
			else
			{
				keyMap[i] = KEY_INPUT_STATUS_NONE;
				change = false;
			}
				
		}
	}


}

Keyboard::Keyboard()
{
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}

Keyboard::~Keyboard()
{

}
