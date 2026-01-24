#pragma once

#include <VelyraCore/Window/KeyboardButtons.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Velyra::Core {

    constexpr auto VL_CORE_WIN32_CLASS_NAME = L"VelyraWindowClass";

    class Win32Instance {
    public:
        static void createInstance();

        static void destroyInstance();

        static Size getInstanceCount();

    private:

        static void initializeCOM();

        static void initializeHInstance();

        static void registerWindowClass();

        static void setProcessDPIAware();

        static void terminateCOM();

        static void unregisterWindowClass();

    private:
        static Size m_InstanceCount;
        static HINSTANCE m_HInstance;
    };

    DWORD decodeWindowStyle(int windowStyle);

    DWORD decodeExtendedWindowStyle(int windowStyle);

    std::wstring toWideString(const std::string& source);

    std::string fromWideString(const std::wstring& source);

    VL_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags);

}