#pragma once

#include "D2DApp.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include "Camera.h"
#include "UploadBuffer.h"
#include "IGameObj.h"
#include "GUI.h"
#include "Keyboard.h"
#include "Timer.h"

struct PassConstants
{
    DirectX::XMFLOAT4X4 View;
    DirectX::XMFLOAT4X4 Proj;
    
};

class MagicTowerApp :public D2DApp, public SingletonBase<MagicTowerApp>
{
private:
    friend class ButtonOnClick;
    friend class SingletonBase<MagicTowerApp>;

    Camera& mCamera = Camera::get_instance();
    Keyboard& mKeyboard = Keyboard::get_instance();
    GUI& mGui = GUI::get_instance();
    Timer& mTimer = Timer::get_instance();

    PassConstants mMainPassCB;

    // constant buffer
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;

public:
    MagicTowerApp();
    MagicTowerApp(const MagicTowerApp& rhs) = delete;
    MagicTowerApp& operator=(const MagicTowerApp& rhs) = delete;
    ~MagicTowerApp();

    virtual HRESULT Initialize()override;


private:
    virtual void OnResize(UINT width, UINT height)override;
    virtual void Update()override;
    virtual void Draw()override;


    void UpdateMainPassCB();

    void BuildResources();

    void DrawRenderItems();


    //virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    // GameObject
private:
    std::unordered_map<int, std::wstring> order;
    std::unordered_map<std::wstring, IGameObj*> mGOs;
    int count = 0;

    // public methods for other class
public:
    void DestroyGO(std::wstring name);
    void Push(std::wstring name, IGameObj* GO);
    void LoadFloor(int floorNum);
};
