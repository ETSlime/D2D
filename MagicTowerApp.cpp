#include "MagicTowerApp.h"
#include "StartMenuGO.h"
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

        Push(L"StartMenuGO", new StartMenuGO(&mD2DResource, &curWindowSize));
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

    // Update Game Objs
    for (auto it = mGOs.begin(); it != mGOs.end();)
    {
        if (it->second->IsDestroyed())
        {
            it = mGOs.erase(it);
        }
        else
        {
            it->second->Update();
            ++it;
        }
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

void MagicTowerApp::Push(std::wstring name, IGameObj* GO)
{
    mGOs[name] = GO;
    //order[count] = name;
    GO->Init();
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

    textRect.left = 0;
    textRect.top = 0;
    textRect.right = 300;
    textRect.bottom = 50;

    mD2DResource.pD2DRenderTarget->DrawText(
        (L"fps: " + fps).c_str(),
        fps.size(),
        mD2DResource.pTextFormat,
        &textRect,
        mD2DResource.pSolidColorBrush);

    // render
    for (const auto& GO : mGOs)
    {
        if (GO.second->IsValid() == true)
            GO.second->Render();
    }

    // post render
    for (const auto& GO : mGOs)
    {
        if (GO.second->IsValid() == true)
            GO.second->PostRender();
    }

    // GUI
    //for (const auto& GO : mGOs)
    //{
    //    if (GO.second->IsValid() == true)
    //        GO.second->GUI();
    //}

    //mGui.Render();

    // For Direct2D
    mD2DResource.pD2DRenderTarget->EndDraw();
}

void MagicTowerApp::DestroyGO(std::wstring name)
{
    mGOs[name]->SetIsDestroyed(true);
}

void MagicTowerApp::LoadFloor(int floorNumber)
{
    
    //Push(L"FloorGO", new FloorGO(floorNumber));
    Push(L"PlayerGO", new PlayerGO(Coord(10,10)));
}