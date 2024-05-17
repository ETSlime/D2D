#pragma once

// Windows Header Files:
#include <windows.h>
#include <comdef.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>


//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dwrite.lib")

//DirectTex
#include <DirectXTex.h>
#include "D3DX11tex.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
//#include <wincodec.h>

//ImGui
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/backends/imgui_impl_dx11.h"
#include "ThirdParty/ImGui/backends/imgui_impl_win32.h"

// Math
#include "MathHelper.h"
#include "XMFLOAT_Helper.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#pragma comment(lib, "d2d1.lib")

// Declare additional functions for releasing interfaces, 
// and macros for error-handling and retrieving the module's base address.
template<class Interface>
inline void SafeRelease(
    Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

#define WinMaxWidth 1280
#define WinMaxHeight 720

#define SAFE_DELETE(p) { if(p) { delete(p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[](p); (p) = nullptr; } }
#define ZERO(p) ZeroMemory(&p, sizeof(p))

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

const UINT TileHeight = 50;
const UINT TileWidth = 50;

const std::wstring ShaderPath = L"_Shaders/";
const std::wstring TexturePath = L"_Textures/";

// Graphics path
const std::wstring SkinsPath = L"Graphics/Windowskins/";
const std::wstring TilesetsPath = L"Graphics/Tilesets/";
const std::wstring PicturesPath = L"Graphics/Pictures/";
const std::wstring IconsPath = L"Graphics/Icons/";
const std::wstring GameoversPath = L"Graphics/Gameovers/";
const std::wstring CharactersPath = L"Graphics/Characters/";
const std::wstring BattlersPath = L"Graphics/Battlers/";
const std::wstring AutotilesPath = L"Graphics/Autotiles/";
const std::wstring AnimationsPath = L"Graphics/Animations/";

// Font
const std::wstring FontPath = L"Fonts/ARIAL.TTF";

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

inline std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

class DxException
{
public:
    DxException() = default;
    DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
        ErrorCode(hr),
        FunctionName(functionName),
        Filename(filename),
        LineNumber(lineNumber)
    {
    }

    std::wstring ToString()const
    {
        // Get the string description of the error code.
        _com_error err(ErrorCode);
        std::wstring msg = err.ErrorMessage();

        return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
    }

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};

struct D2DResource
{
    ID2D1RenderTarget* pD2DRenderTarget;
    IDWriteTextFormat* pTextFormat;
    ID2D1SolidColorBrush* pSolidColorBrush;
};

struct WinSize
{
    UINT width = 0;
    UINT height = 0;
};

struct Coord
{
    UINT x;
    UINT y;

    Coord(UINT x, UINT y) :x(x), y(y) {}

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

// hash function for Coord key
namespace std {
    template <>
    struct hash<Coord> {
        std::size_t operator()(const Coord& c) const noexcept {
            return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
        }
    };
}