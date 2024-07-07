#include "MagicTowerApp.h"
#include "OffScreenRenderer.h"
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
    gameMode = GameMode::TITLE;
}

MagicTowerApp::~MagicTowerApp()
{
    SAFE_DELETE(offScreenRenderer);
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

    }
    return hr;
}

void MagicTowerApp::BuildResources()
{
    gameUI = std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::INGAMEUI);
    startMenuGO = std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::STARTMENU);
    startMenuGO->Init();
    PassCB = std::make_unique<UploadBuffer<PassConstants>>(
        mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);

    offScreenRenderer = new OffScreenRenderer(pD2DFactory, &mD2DResource, OffScreenRenderMode::NONE, EffectParameters(),
        DirectX::XMFLOAT3(curWindowSize.width, 0.0f, 0.0f), DirectX::XMFLOAT3(curWindowSize.width, curWindowSize.height, 1.0f), 0.0f);
}

void MagicTowerApp::UpdateMainPassCB()
{
    DirectX::XMMATRIX view = mCamera.GetView();
    DirectX::XMMATRIX proj = mCamera.GetProj();

    // Update view port
    XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
    XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));

    PassCB->MapData(mDeviceContext, mMainPassCB);

}

void MagicTowerApp::Push(std::wstring name, std::unique_ptr<IGameObj> GO)
{
    GO->Init();
    mGOs[name] = std::move(GO);
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
        if (it->second && it->second->IsDestroyed())
        {
            it->second->Destroy();
            it = mGOs.erase(it);
        }
        else
            it++;
    }

    switch (gameMode)
    {
    case GameMode::TITLE:
        if (gameUI)
        {
            if (gameUI->IsDestroyed())
            {
                gameUI->Destroy();
                gameUI.release();
            }
            else if (gameUI->IsValid())
                gameUI->Update();
        }
        if (startMenuGO)
        {
            if (startMenuGO->IsDestroyed())
            {
                startMenuGO->Destroy();
                startMenuGO.release();
            }
            else if (startMenuGO->IsValid())
                startMenuGO->Update();
        }

        if (mGOs[L"PlayerGO"] && mGOs[L"PlayerGO"].get()->IsValid())
            mGOs[L"PlayerGO"]->Update();
        break;
    case GameMode::GAMEPLAY:
        // processing queue
        // push queue
        while (!pushQueue.empty()) 
        {
            std::wstring GOName = pushQueue.front();
            if (GOName.compare(0, 6, L"FloorGO"))
                Push(GOName, std::make_unique<FloorGO>(std::stoi(GOName.substr(7))));
            pushQueue.pop();
        }
        // dialogue queue
        if (!dialogueQueue.empty() && mGOs.find(L"UIDialogueGO") == mGOs.end())
        {
            Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::DIALOGUE));
            // dialogue name
            dynamic_cast<GameUIGO*>(mGOs[L"UIDialogueGO"].get())->SetDialogueName(std::get<1>(dialogueQueue.front()));
            // dialogue context
            dynamic_cast<GameUIGO*>(mGOs[L"UIDialogueGO"].get())->SetDialogue(std::get<2>(dialogueQueue.front()), std::get<0>(dialogueQueue.front()));
            dialogueQueue.pop();
        }

        // update GO
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


    if (enableShakeEffect)
    {
        EffectParameters effectParams;
        effectParams.shakeIntensity = 0.01f; // Set shake intensity
        offScreenRenderer->SetEffectMode(OffScreenRenderMode::SHAKE);
        offScreenRenderer->SetEffectParameters(effectParams);
        OffScreenEffectRender();
    }
    else if (enableFadeEffect)
    {
        EffectParameters effectParams;
        effectParams.fadeAlpha = 0.5f; // Set fade alpha
        offScreenRenderer->SetEffectMode(OffScreenRenderMode::FADE);
        offScreenRenderer->SetEffectParameters(effectParams);
        OffScreenEffectRender();
    }
    else if (enableBlurEffect)
    {
        EffectParameters effectParams;
        effectParams.blurAmount = 0.5f; // Set blur amount
        offScreenRenderer->SetEffectMode(OffScreenRenderMode::BLUR);
        offScreenRenderer->SetEffectParameters(effectParams);
        OffScreenEffectRender();
    }
    else 
    {
        // Switch pD2DRenderTarget back to swap chain back buffer
        // Release the existing Direct2D render target if it exists
        if (mD2DResource.pD2DRenderTarget)
        {
            SafeRelease(&mD2DResource.pD2DRenderTarget);
        }
        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
        pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBufferSurface, &props, &(mD2DResource.pD2DRenderTarget));

        // Specify the buffers we are going to render to.
        mDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);

        // Do rendering stuffs...
        DrawRenderItems();
    }


    // Swap the back and front buffers
    HRESULT hr = mSwapChain->Present(bVsync == true ? 1 : 0, 0);
    assert(SUCCEEDED(hr));
}


void MagicTowerApp::OffScreenEffectRender()
{
    offScreenRenderer->Begin();

    // Do rendering stuffs...
    DrawRenderItems();

    // Reset render target to the screen
    mDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);

    // Update offScreenRenderer
    offScreenRenderer->Update();
    // Render
    offScreenRenderer->ApplyEffect(offScreenRenderer->GetEffectParams());

    // Restore blend state
    mDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
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
    case GameMode::TITLE:
        if (startMenuGO && startMenuGO->IsValid() == true)
            startMenuGO->Render();
        if (mGOs[L"PlayerGO"])
            mGOs[L"PlayerGO"]->Render();
        if (gameUI && gameUI->IsValid() == true)
            gameUI->Render();
        break;
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
    else if (name == L"StartMenuGO")
        startMenuGO->SetIsDestroyed(true);
}

void MagicTowerApp::SetValidGO(std::wstring name, bool valid)
{
    if (mGOs[name])
        mGOs[name]->SetIsValid(valid);
    else if (name == L"StartMenuGO")
        startMenuGO->SetIsValid(valid);
    else if (name == L"GameUIGO")
        gameUI->SetIsValid(valid);
        
}

void MagicTowerApp::LoadFloor(int floorNumber)
{
    std::wstring floorGOName = L"FloorGO" + std::to_wstring(floorNumber);
    pushQueue.push(floorGOName);
    Push(L"MainGAMEUIGO", std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::PLAYERSTATES));
}

void MagicTowerApp::SetGameMode(GameMode mode)
{
    gameMode = mode;
    switch (mode)
    {
    case GameMode::TITLE:
    {
        startMenuGO->Init();
        break;
    }
    case GameMode::DISPLAYMENU:
        dynamic_cast<GameUIGO*>(gameUI.get())->ChangeUIMode(GameUI::UIRenderMode::INGAMEUI);
        break;
      
    case GameMode::GAMEPLAY:
        if (gameUI == nullptr)
            gameUI = std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::INGAMEUI);
        gameUI->Init();
        break;
    }
}

void MagicTowerApp::ReturnTitle()
{
    MapStatic::eventFloor->clear();
    gameUI->SetIsDestroyed(true);
    for (auto& GO : mGOs)
    {
        if (GO.second)
            GO.second->SetIsDestroyed(true);
    }
}

void MagicTowerApp::ShowMessageDialogue(UINT itemID)
{
    Push(L"UIDialogueGO", std::make_unique<GameUIGO>(&mD2DResource, &curWindowSize, GameUI::MESSAGE));
    dynamic_cast<GameUIGO*>(mGOs[L"UIDialogueGO"].get())->SetDialogue(Database::itemGetText[itemID]);
}

void MagicTowerApp::ShowNPCDialogue(UINT dialogueID)
{
    std::vector<std::tuple<std::vector<DialogueButtonEvent>, std::wstring, std::wstring>>& dialogues = Database::dialogues[dialogueID];
    
    for (auto& dialogue : dialogues)
    {
        dialogueQueue.push(dialogue);
    }
}