#pragma once

#include "IGameObj.h"
#include "MagicTowerApp.h"
#include "GameUI.h"

class GameUIGO : public IGameObj
{
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	virtual bool IsValid() override;
	virtual bool IsDestroyed() override;
	virtual void SetIsDestroyed(bool _destroy) override;
	virtual void SetIsValid(bool _valid) override;

	GameUIGO(const D2DResource* D2DResource, const WinSize* winSize, GameUI::UIRenderMode mode);

	void ChangeUIMode(GameUI::UIRenderMode mode);
	void SetChangeUIMode() { gameUI->SetRenderModeOnChanging(); }
	void SetChangeGameMode(bool change) { gameUI->SetGameModeOnChanging(change); }
private:

	class GameUI* gameUI = nullptr;
	GameUI::UIRenderMode UImode;
	bool valid = true;
	bool destroy = false;

	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
	const D2DResource* mD2DResource = nullptr;
	const WinSize* curWinSize = nullptr;
};