#include "MagicTowerApp.h"
#include "TextureDemo.h"

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
            MagicTowerApp app;

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
        // set up camera
        mCamera.LookAtLH(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 1, 0));
        mCamera.OffCenterLH(0.0f, (float)mClientWidth, 0.0f, (float)mClientHeight, 0, 1);

        // Init GUI
        mGui.Init(m_hwnd, mDevice, mDeviceContext);

        BuildResources();

        Push(new TextureDemo(mDevice, mDeviceContext));
        //Push(new PlayerDemo());
    }
    return hr;
}

void MagicTowerApp::OnResize(UINT width, UINT height)
{
    D2DApp::OnResize(width, height);
}

void MagicTowerApp::Update()
{
    // camera view matrix
    //mCamera.UpdateViewMatrix();

    UpdateMainPassCB();

    mGui.Update();

    if (mGOs[count]->IsValid() == true)
        mGOs[count]->Update();
    else
        count++;


    if (count == mGOs.size())
    {
        count = 0;
        for (IGameObj* GO : mGOs)
        {
            GO->SetIsValid(true);
        }

    }

    
}

void MagicTowerApp::Draw()
{
    mDeviceContext->RSSetViewports(1, &mScreenViewport);

    // Clear the back buffer (no depth buffer)
    mDeviceContext->ClearRenderTargetView(rtv, DirectX::Colors::LightSteelBlue);
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

    XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
    XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));

    PassCB->MapData(mDeviceContext,mMainPassCB);

}

void MagicTowerApp::Push(IGameObj* GO)
{
    mGOs.push_back(GO);
    GO->Init();
}

void MagicTowerApp::DrawRenderItems()
{
    

    mDeviceContext->VSSetConstantBuffers(1, 1, PassCB->Resource());

    // render
    if (mGOs[count]->IsValid() == true)
        mGOs[count]->Render();

    // post render
    if (mGOs[count]->IsValid() == true)
        mGOs[count]->PostRender();
    // GUI
    if (mGOs[count]->IsValid() == true)
        (*mGOs[count]).GUI();

    mGui.Render();
}