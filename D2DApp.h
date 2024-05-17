#include "Util.h"

class D3DEnumOutputInfo
{
public:
    ~D3DEnumOutputInfo();

    IDXGIOutput* output = nullptr;
    DXGI_OUTPUT_DESC outputDesc = { 0 };

    UINT numerator = 0;
    UINT denominator = 1;
};

class D3DEnumAdapterInfo
{
public:
    ~D3DEnumAdapterInfo();

    UINT adapterOrdinal = 0;
    IDXGIAdapter1* adapter = nullptr;
    DXGI_ADAPTER_DESC1 adapterDesc = { 0 };

    D3DEnumOutputInfo* outputInfo = nullptr;
};

class D2DApp
{
public:

    D2DApp();
    ~D2DApp();

    // Register the window class and call methods for instantiating drawing resources
    virtual HRESULT Initialize();

    // Process and dispatch messages
    void Run();

    static D2DApp* GetApp();

private:
    // Initialize device-independent resources.
    HRESULT CreateDeviceIndependentResources();

    // Initialize device-dependent resources.
    HRESULT CreateRenderTargetView();

    // Release device-dependent resource.
    void DiscardDeviceResources();

protected:
    // Draw content.
    HRESULT OnRender();
    virtual void Draw() = 0;
    virtual void Update() = 0;

    // Resize the render target.
    virtual void OnResize(
        UINT width,
        UINT height
    );
    
public:

    static D2DApp* mApp;
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;

protected:

    HWND m_hwnd = nullptr; // main window handle
    HINSTANCE mhAppInst = nullptr; // application instance handle
    bool mAppPaused = false;  // is the application paused?
    bool mMinimized = false;  // is the application minimized?
    bool mMaximized = false;  // is the application maximized?
    bool mResizing = false;   // are the resize bars being dragged?
    bool mFullscreenState = false;// fullscreen enabled

    // Direct3D
    IDXGIFactory1* mdxgiFactory = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    D3D11_VIEWPORT mScreenViewport;

    // Direct2D
    ID2D1Factory* pD2DFactory = nullptr;
    IDWriteFactory* pD2DWriteFactory = nullptr;
    D2DResource mD2DResource;
    float baseFontSize = 32.0f;
    float aspectRatio = 1.0f;
    WinSize curWindowSize = { WinMaxWidth, WinMaxHeight };


    bool bVsync = true;
    std::wstring gpuName;

    std::vector<D3DEnumAdapterInfo*> adapterInfos;
    int selectedAdapterIndex = 0;

    void LogAdapters();
    bool LogAdapterOutputs(D3DEnumAdapterInfo* adapterInfo);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

    void CreateSwapchain();

    // ID2D
    //ID2D1Factory* m_pDirect2dFactory;
    //ID2D1HwndRenderTarget* m_pRenderTarget;
    //ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    //ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

    // The windows procedure.
    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Derived class should set these in derived constructor to customize starting values.
    std::wstring mMainWndCaption = L"d2d App";
    D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    int mClientWidth = WinMaxWidth;
    int mClientHeight = WinMaxHeight;
};

