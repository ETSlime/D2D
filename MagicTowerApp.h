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
#include "Database.h"

class OffScreenRenderer;

enum class GameMode
{
    TITLE,
    GAMEPLAY,
    DISPLAYMENU
};

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
        // Check if lhs and rhs start with "UI"
        bool lhs_is_UI = lhs.compare(0, 2, L"UI") == 0;
        bool rhs_is_UI = rhs.compare(0, 2, L"UI") == 0;

        // Check if lhs and rhs start with "Player"
        bool lhs_is_player = lhs.compare(0, 6, L"Player") == 0;
        bool rhs_is_player = rhs.compare(0, 6, L"Player") == 0;

        // Check if lhs and rhs start with "Event"
        bool lhs_is_event = lhs.compare(0, 5, L"Event") == 0;
        bool rhs_is_event = rhs.compare(0, 5, L"Event") == 0;

        // Prioritize "UI" to be at the end
        if (lhs_is_UI && !rhs_is_UI) return false;
        if (!lhs_is_UI && rhs_is_UI) return true;

        // "Player" should be after other strings but before "UI"
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
    friend class GeneralEventOnTrigger;

    Camera& mCamera = Camera::get_instance();
    Keyboard& mKeyboard = Keyboard::get_instance();
    Timer& mTimer = Timer::get_instance();

    PassConstants mMainPassCB{};
    OffScreenRenderer* offScreenRenderer = nullptr;
    bool enableShakeEffect = false, enableFadeEffect = false, enableBlurEffect = false;

    // constant buffer
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;

    GameMode gameMode;

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

    void OffScreenEffectRender();

    //virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    // GameObject
private:
    std::map<std::wstring, std::unique_ptr<IGameObj>, DrawingOrder> mGOs;
    std::unique_ptr<IGameObj> gameUI;
    std::unique_ptr<IGameObj> startMenuGO;
    std::queue<std::wstring> pushQueue;
    std::queue<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>> dialogueQueue;

    bool allowSwitchMode = true;



public:
    // public methods for other class
    void ShowMessageDialogue(UINT itemID);
    void ShowNPCDialogue(UINT dialogueID);
    void ReturnTitle();
    void DestroyGO(std::wstring name);
    void SetValidGO(std::wstring name, bool valid);
    void Push(std::wstring name, std::unique_ptr<IGameObj> GO);
    void LoadFloor(int floorNum);
    void SetGameMode(GameMode mode);
    GameMode GetGameMode() { return gameMode; }
    bool AvailableToSwitch() { return allowSwitchMode; }
    void SetAllowSwitch(bool allow) { allowSwitchMode = allow; }
    void SetShakeEffect(bool enable) { enableShakeEffect = enable; }
    void SetFadeEffect(bool enable) { enableFadeEffect = enable; }
    void SetBlueEffect(bool enable) { enableBlurEffect = enable; }
};
