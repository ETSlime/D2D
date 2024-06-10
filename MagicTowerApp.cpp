#include "MagicTowerApp.h"
#include "GameUIGO.h"
#include "FloorGO.h"
#include "PlayerGO.h"

int main()
{
    // Use HeapSetInformation to specify that the process should
    // terminate if the heap manager detects an error in any heap used
    // by the process.
    // The return value is ignored, because we want to continue running in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            MagicTowerApp& app = MagicTowerApp::get_instance();

            if (SUCCEEDED(app.Initialize()))
            {
                app.Run();
            }
        }
        CoUninitialize();
    }

    return 0;
}

MagicTowerApp::MagicTowerApp():D2DApp()
{
    gameMode = GameMode::GAMEPLAY;
}

MagicTowerApp::~MagicTowerApp()
{

}

HRESULT MagicTowerApp::Initialize()
{
    HRESULT hr;
    hr = D2DApp::Initialize();

    if (SUCCEEDED(hr))
    {
        mTimer.Start();

        // set up camera
        mCamera.LookAtLH(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 1, 0));
        mCamera.OffCenterLH(0.0f, (float)mClientWidth, 0.0f, (float)mClientHeight, 0, 1);

        // Init GUI
        //mGui.Init(m_hwnd, mDevice, mDeviceContext);

        BuildResources();

        Push(L"StartMenuGO", std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::STARTMENU));
    }
    return hr;
}

void MagicTowerApp::OnResize(UINT width, UINT height)
{
    D2DApp::OnResize(width, height);
}

void MagicTowerApp::Update()
{
    // Update timer
    mTimer.Update();
    mKeyboard.Update();

    // camera view matrix
    //mCamera.UpdateViewMatrix();

    UpdateMainPassCB();

    //// Update GUI
    //mGui.Update();

    switch (gameMode)
    {
    case GameMode::GAMEPLAY:
        // Update Game Objs
        for (auto it = mGOs.begin(); it != mGOs.end();)
        {
            if (it->second && it->second->IsDestroyed())
            {
                it->second->Destroy();
                it = mGOs.erase(it);
            }
            else
                it++;
        }
        for (auto it = pushQueue.begin(); it != pushQueue.end();)
        {
            Push(*it, std::make_unique<FloorGO>(std::stoi((*it).substr(7))));
            it = pushQueue.erase(it);
        }

        for (auto it = mGOs.begin(); it != mGOs.end();)
        {
            if (it->second)
                it->second->Update();
            ++it;
        }
        break;
    case GameMode::DISPLAYMENU:
        gameUI->Update();
        break;
    }


}

void MagicTowerApp::Draw()
{
    mDeviceContext->RSSetViewports(1, &mScreenViewport);

    // Clear the back buffer (no depth buffer)
    mDeviceContext->ClearRenderTargetView(rtv, DirectX::Colors::Black);
    // Specify the buffers we are going to render to.
    mDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);

    // Do rendering stuffs...
    DrawRenderItems();


    // Swap the back and front buffers
    HRESULT hr = mSwapChain->Present(bVsync == true ? 1 : 0, 0);
    assert(SUCCEEDED(hr));
}

void MagicTowerApp::BuildResources()
{
    gameUI = std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::INGAMEUI);

    PassCB = std::make_unique<UploadBuffer<PassConstants>>(
        mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
}

void MagicTowerApp::UpdateMainPassCB()
{
    DirectX::XMMATRIX view = mCamera.GetView();
    DirectX::XMMATRIX proj = mCamera.GetProj();

    // Update view port
    XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
    XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));

    PassCB->MapData(mDeviceContext,mMainPassCB);

}

void MagicTowerApp::Push(std::wstring name, std::unique_ptr<IGameObj> GO)
{
    GO->Init();
    mGOs[name] = std::move(GO);
}

void MagicTowerApp::DrawRenderItems()
{
    

    mDeviceContext->VSSetConstantBuffers(1, 1, PassCB->Resource());

    mKeyboard.Update();

    // For Direct2D
    mD2DResource.pD2DRenderTarget->BeginDraw();

    // fps
    std::wstring fps(std::to_wstring(mTimer.FPS()));
    D2D1_RECT_F textRect;

    textRect.left = 20;
    textRect.top = WinMaxHeight - 50;
    textRect.right = 300;
    textRect.bottom = WinMaxHeight;

    mD2DResource.pD2DRenderTarget->DrawText(
        (L"fps: " + fps).c_str(),
        fps.size(),
        mD2DResource.pTextFormat,
        &textRect,
        mD2DResource.pSolidColorBrush);

    switch (gameMode)
    {
    case GameMode::GAMEPLAY:
        // render
        for (const auto& GO : mGOs)
        {
            if (GO.second && GO.second->IsValid() == true)
                GO.second->Render();
        }

        // post render
        for (const auto& GO : mGOs)
        {
            if (GO.second && GO.second->IsValid() == true)
                GO.second->PostRender();
        }
        break;
    case GameMode::DISPLAYMENU:
        gameUI->Render();
        break;
    }


    // For Direct2D
    mD2DResource.pD2DRenderTarget->EndDraw();
}

void MagicTowerApp::DestroyGO(std::wstring name)
{
    if (mGOs[name])
        mGOs[name]->SetIsDestroyed(true);
}

void MagicTowerApp::LoadFloor(int floorNumber)
{
    std::wstring floorGOName = L"FloorGO" + std::to_wstring(floorNumber);
    pushQueue.push_back(floorGOName);
    Push(L"MainGAMEUIGO", std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::PLAYERSTATES));
}