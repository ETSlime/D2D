#include "D2DApp.h"



// for SUBSYSTEM:WINDOWS
//int WINAPI WinMain(
//    HINSTANCE /* hInstance */,
//    HINSTANCE /* hPrevInstance */,
//    LPSTR /* lpCmdLine */,
//    int /* nCmdShow */
//)
//{
//
//}


D2DApp* D2DApp::mApp = nullptr;
D2DApp* D2DApp::GetApp()
{
    return mApp;
}

D2DApp::D2DApp()
{
    // Only one D3DApp can be constructed.
    assert(mApp == nullptr);
    mApp = this;

    mScreenViewport = { 0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight, 0.0f, 1.0f };
}

D2DApp::~D2DApp()
{
    SafeRelease(&mdxgiFactory);
    SafeRelease(&rtv);
}

HRESULT D2DApp::Initialize()
{
    HRESULT hr;

    // Initialize device-indpendent resources, such
    // as the Direct2D factory.
    hr = CreateDeviceIndependentResources();


    if (SUCCEEDED(hr))
    {
        // Register the window class.
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = D2DApp::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = mhAppInst;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"D2DDemoApp";

        RegisterClassEx(&wcex);

        // Compute window rectangle dimensions based on requested client area dimensions.
        RECT R = { 0, 0, mClientWidth, mClientHeight };
        AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
        int width = R.right - R.left;
        int height = R.bottom - R.top;

        // In terms of using the correct DPI, to create a window at a specific size
        // like this, the procedure is to first create the window hidden. Then we get
        // the actual DPI from the HWND (which will be assigned by whichever monitor
        // the window is created on). Then we use SetWindowPos to resize it to the
        // correct DPI-scaled size, then we use ShowWindow to show it.

        m_hwnd = CreateWindow(
            wcex.lpszClassName,
            L"Magic Tower",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            0,
            0,
            mhAppInst,
            0);
        if (!m_hwnd)
        {
            MessageBox(0, L"CreateWindow Failed.", 0, 0);
            return hr;
        }
        if (m_hwnd)
        {
            // Because the SetWindowPos function takes its size in pixels, we
            // obtain the window's DPI, and use it to scale the window size.
            float dpi = GetDpiForWindow(m_hwnd);

            SetWindowPos(
                m_hwnd,
                NULL,
                NULL,
                NULL,
                static_cast<int>(ceil(mClientWidth * dpi / 96.f)),
                static_cast<int>(ceil(mClientHeight * dpi / 96.f)),
                SWP_NOMOVE);
            ShowWindow(m_hwnd, SW_SHOW);
            UpdateWindow(m_hwnd);
        }

        aspectRatio = static_cast<float>(mClientWidth) / static_cast<float>(mClientHeight);

        //ShowWindow(m_hwnd, SW_SHOW);
        //UpdateWindow(m_hwnd);
        //ShowCursor(true);//

#ifdef _DEBUG
        LogAdapters();
#endif
        CreateSwapchain();

        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.IndependentBlendEnable = false; // 只使用第一个渲染目标的混合状态
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        // 创建混合状态对象
        ID3D11BlendState* blendState = nullptr;
        mDevice->CreateBlendState(&blendDesc, &blendState);

        // 设置混合状态
        mDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);

    }

    return hr;
}

void D2DApp::Run()
{
    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Otherwise, do animation/game stuff.
        else
        {
            if (!mAppPaused)
            {
                Update();
                Draw();
            }
            else
            {
                Sleep(100);
            }
        }
    }
}

HWND handle;
LRESULT CALLBACK D2DApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return D2DApp::GetApp()->MsgProc(hwnd, message, wParam, lParam);
}

LRESULT D2DApp::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    if (message == WM_CREATE)
    {
        ::handle = hwnd;

        result = 1;
    }
    else
    {
        bool wasHandled = false;

        if (mDevice)
        {
            switch (message)
            {
            case WM_SIZE:
            {
                // Save the new client area dimensions.
                mClientWidth = LOWORD(lParam);
                mClientHeight = HIWORD(lParam);
                if (mDevice)
                {
                    if (wParam == SIZE_MINIMIZED)
                    {
                        mAppPaused = true;
                        mMinimized = true;
                        mMaximized = false;
                    }
                    else if (wParam == SIZE_MAXIMIZED)
                    {
                        mAppPaused = false;
                        mMinimized = false;
                        mMaximized = true;
                        OnResize(mClientWidth, mClientHeight);
                    }
                    else if (wParam == SIZE_RESTORED)
                    {
                        // Restoring from minimized state?
                        if (mMinimized)
                        {
                            mAppPaused = false;
                            mMinimized = false;
                            OnResize(mClientWidth, mClientHeight);
                        }
                        // Restoring from maximized state?
                        else if (mMaximized)
                        {
                            mAppPaused = false;
                            mMaximized = false;
                            OnResize(mClientWidth, mClientHeight);
                        }
                        else if (mResizing)
                        {
                            // If user is dragging the resize bars, we do not resize 
                            // the buffers here because as the user continuously 
                            // drags the resize bars, a stream of WM_SIZE messages are
                            // sent to the window, and it would be pointless (and slow)
                            // to resize for each WM_SIZE message received from dragging
                            // the resize bars.  So instead, we reset after the user is 
                            // done resizing the window and releases the resize bars, which 
                            // sends a WM_EXITSIZEMOVE message.
                        }
                        else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
                        {
                            OnResize(mClientWidth, mClientHeight);
                        }
                    }
                }
            }
            result = 0;
            wasHandled = true;
            break;
            // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
            case WM_ENTERSIZEMOVE:
                mAppPaused = true;
                mResizing = true;
                //mTimer.Stop();
                return 0;

            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
            case WM_EXITSIZEMOVE:
                mAppPaused = false;
                mResizing = false;
                //mTimer.Start();
                OnResize(mClientWidth, mClientHeight);
                return 0;

            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_PAINT:
            {
                ValidateRect(hwnd, NULL);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            result = 1;
            wasHandled = true;
            break;
            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

HRESULT D2DApp::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    assert(SUCCEEDED(hr));

    // Create a DXGI factory.
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&mdxgiFactory);
    assert(SUCCEEDED(hr));

    // Create D2DWrite factory
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pD2DWriteFactory));
    assert(SUCCEEDED(hr));

    // Create font
    hr = pD2DWriteFactory->CreateTextFormat(FontPath.c_str(), nullptr, 
        DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, baseFontSize, L"", &(mD2DResource.pTextFormat));
    assert(SUCCEEDED(hr));

    // Set alignment
    mD2DResource.pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    mD2DResource.pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


    return hr;
}



HRESULT D2DApp::CreateRenderTargetView()
{

    ID3D11Texture2D* backbuffer = nullptr;
    HRESULT hr = mSwapChain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backbuffer
    );
    assert(SUCCEEDED(hr));

    hr = mDevice->CreateRenderTargetView
    (
        backbuffer,
        nullptr,
        &rtv
    );
    assert(SUCCEEDED(hr));

    IDXGISurface* pBackBufferSurface;
    backbuffer->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&pBackBufferSurface));
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBufferSurface, &props, &(mD2DResource.pD2DRenderTarget));
    assert(SUCCEEDED(hr));

    SafeRelease(&pBackBufferSurface);
    SafeRelease(&backbuffer);

    return hr;
}

void D2DApp::DiscardDeviceResources()
{
    SafeRelease(&rtv);
    SafeRelease(&mSwapChain);
    SafeRelease(&mDeviceContext);
    SafeRelease(&mDevice);

    // D2D resources
    SafeRelease(&mD2DResource.pD2DRenderTarget);
    SafeRelease(&mD2DResource.pSolidColorBrush);

    //SafeRelease(&m_pRenderTarget);
    //SafeRelease(&m_pLightSlateGrayBrush);
    //SafeRelease(&m_pCornflowerBlueBrush);
}

HRESULT D2DApp::OnRender()
{
    HRESULT hr = S_OK;

    hr = CreateRenderTargetView();

    // Verify that the CreateDeviceResource method
    if (SUCCEEDED(hr))
    {

        // Start draw
        Draw();
    }

    // Recreated when the window receives a WM_PAINT or WM_DISPLAYCHANGE message
    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}

void D2DApp::OnResize(UINT width, UINT height)
{
    assert(mDevice);
    assert(mSwapChain);

    // Adjust windows size according to aspect ratio
    if (width / height > aspectRatio) 
    {
        height = static_cast<UINT>(width / aspectRatio);
    }
    else 
    {
        width = static_cast<UINT>(height * aspectRatio);
    }
    float winScale = width  / 1280.0f;

    curWindowSize.width = width;
    curWindowSize.height = height;

    SafeRelease(&rtv);

    // D2D
    SafeRelease(&mD2DResource.pD2DRenderTarget);
    SafeRelease(&mD2DResource.pSolidColorBrush);
    SafeRelease(&mD2DResource.pTextFormat);

    HRESULT hr = mSwapChain->ResizeBuffers
    (
        1,
        width,
        height,
        DXGI_FORMAT_B8G8R8A8_UNORM,//DXGI_FORMAT_UNKNOWN,
        0
    );
    assert(SUCCEEDED(hr));

    CreateRenderTargetView();

    // SetViewport
    mScreenViewport.TopLeftX = 0.0f;
    mScreenViewport.TopLeftY = 0.0f;
    mScreenViewport.Width = static_cast<float>(width);
    mScreenViewport.Height = static_cast<float>(height);
    mScreenViewport.MinDepth = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;

    hr = pD2DWriteFactory->CreateTextFormat(FontPath.c_str(), 
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL, // font weight
        DWRITE_FONT_STYLE_NORMAL, // font style
        DWRITE_FONT_STRETCH_CONDENSED, // font strech
        baseFontSize * winScale, // font size
        L"", // font language
        &(mD2DResource.pTextFormat));
    // Set alignment
    mD2DResource.pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    mD2DResource.pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    mD2DResource.pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &(mD2DResource.pSolidColorBrush));
}

void D2DApp::LogAdapters()
{
    UINT index = 0;
    while (true)
    {
        IDXGIAdapter1* adapter = nullptr;
        HRESULT hr = mdxgiFactory->EnumAdapters1(index, &adapter);
        if (hr == DXGI_ERROR_NOT_FOUND)
            break;
        assert(SUCCEEDED(hr));

        D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
        ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
        adapterInfo->adapterOrdinal = index;

        adapter->GetDesc1(&adapterInfo->adapterDesc);
        adapterInfo->adapter = adapter;

        LogAdapterOutputs(adapterInfo);
        adapterInfos.push_back(adapterInfo);

        index++;
    }
    SafeRelease(&mdxgiFactory);
}

bool D2DApp::LogAdapterOutputs(D3DEnumAdapterInfo* adapterInfo)
{
    IDXGIOutput* output = nullptr;
    HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);

    if (DXGI_ERROR_NOT_FOUND == hr)
        return false;
    assert(SUCCEEDED(hr));

    D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();
    ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));

    output->GetDesc(&outputInfo->outputDesc);
    outputInfo->output = output;

    UINT numModes = 0;
    DXGI_MODE_DESC* displayModes = nullptr;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    assert(SUCCEEDED(hr));

    displayModes = new DXGI_MODE_DESC[numModes];
    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
    assert(SUCCEEDED(hr));

    for (UINT i = 0; i < numModes; i++)
    {
        bool bCheck = true;
        bCheck &= displayModes[i].Width == mClientWidth;
        bCheck &= displayModes[i].Height == mClientHeight;

        if (bCheck == true)
        {
            outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
            outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
        }
    }

    adapterInfo->outputInfo = outputInfo;
    SAFE_DELETE_ARRAY(displayModes);
    return true;
}

void D2DApp::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
    UINT count = 0;
    UINT flags = 0;

    // Call with nullptr to get list count.
    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);

    for (auto& x : modeList)
    {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        ::OutputDebugString(text.c_str());
    }
}

void D2DApp::CreateSwapchain()
{
    SafeRelease(&mDevice);
    SafeRelease(&mDeviceContext);
    SafeRelease(&mSwapChain);

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    desc.BufferDesc.Width = mClientWidth;
    desc.BufferDesc.Height = mClientHeight;

    if (bVsync)
    {
        desc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
        desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
    }
    else
    {
        desc.BufferDesc.RefreshRate.Numerator = 0;
        desc.BufferDesc.RefreshRate.Denominator = 1;
    }

    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.ScanlineOrdering
        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferCount = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.OutputWindow = handle;
    desc.Windowed = true;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;



    std::vector<D3D_FEATURE_LEVEL> featureLevel
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT maxVRam = 0;
    for (UINT i = 0; i < adapterInfos.size(); i++)
    {
        if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVRam)
        {
            selectedAdapterIndex = i;
            maxVRam = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
        }
    }

    for (UINT i = 0; i < adapterInfos.size(); i++)
    {
        gpuName = adapterInfos[i]->adapterDesc.Description;
        std::wcout << "GPU Name : " << adapterInfos[i]->adapterDesc.Description << std::endl;
        std::cout << "VRAM : " << adapterInfos[i]->adapterDesc.DedicatedVideoMemory << std::endl;
        std::cout << std::endl;
    }

    std::wcout << "Selected GPU Name : "
        << adapterInfos[selectedAdapterIndex]->
        adapterDesc.Description << std::endl;

    // NEED this flag for Direct2D interoperability with Direct3D resources.
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        adapterInfos[selectedAdapterIndex]->adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        createDeviceFlags,
        featureLevel.data(),
        featureLevel.size(),
        D3D11_SDK_VERSION,
        &desc,
        &mSwapChain,
        &mDevice,
        nullptr,
        &mDeviceContext
    );
    assert(SUCCEEDED(hr));

    OnResize(mClientWidth, mClientHeight);
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
    SafeRelease(&adapter);
    SAFE_DELETE(outputInfo);
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
    SafeRelease(&output);
}