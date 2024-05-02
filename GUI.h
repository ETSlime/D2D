#pragma once

#include "Util.h"
#include "SingletonBase.h"



class GUI : public SingletonBase<GUI>
{
public:
	friend class SingletonBase<GUI>;

	~GUI();

	LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void Update();
	void Render();

	void Init(HWND handle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	GUI();
	

private:
	void ApplyStyle();
};