#pragma once

#include "D2DApp.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include "Camera.h"
#include "UploadBuffer.h"
#include "IGameObj.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Coroutine.h"

struct PassConstants
{
    DirectX::XMFLOAT4X4 View;
    DirectX::XMFLOAT4X4 Proj;

    PassConstants() = default;
    
};

// Custom comparison for drawing game objects
struct DrawingOrder 
{
    bool operator()(const std::wstring& lhs, const std::wstring& rhs) const 
    {
        // Check if lhs and rhs start with "Player"
        bool lhs_is_player = lhs.compare(0, 6, L"Player") == 0;
        bool rhs_is_player = rhs.compare(0, 6, L"Player") == 0;

        // Check if lhs and rhs start with "Event"
        bool lhs_is_event = lhs.compare(0, 5, L"Event") == 0;
        bool rhs_is_event = rhs.compare(0, 5, L"Event") == 0;

        // Prioritize "Player" to be at the end
        if (lhs_is_player && !rhs_is_player) return false;
        if (!lhs_is_player && rhs_is_player) return true;

        // "Event" should be after other strings but before "Player"
        if (lhs_is_event && !rhs_is_event) return false;
        if (!lhs_is_event && rhs_is_event) return true;

        // For other cases, sort alphabetically
        return lhs < rhs;
    }
};

class MagicTowerApp :public D2DApp, public SingletonBase<MagicTowerApp>
{
private:
    friend class ButtonOnClick;
    friend class SingletonBase<MagicTowerApp>;

    Camera& mCamera = Camera::get_instance();
    Keyboard& mKeyboard = Keyboard::get_instance();
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
    std::map<std::wstring, std::unique_ptr<IGameObj>, DrawingOrder> mGOs;

    // public methods for other class
public:
    void DestroyGO(std::wstring name);
    void Push(std::wstring name, std::unique_ptr<IGameObj> GO);
    void LoadFloor(int floorNum);

    Coroutine& coro = Coroutine::get_instance();
};
