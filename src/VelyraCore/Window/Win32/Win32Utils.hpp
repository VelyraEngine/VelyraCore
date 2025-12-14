#pragma once

#include <VelyraCore/Window/KeyboardButtons.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Velyra::Core {

    void setProcessDPIAware();

    DWORD decodeWindowStyle(int windowStyle);

    DWORD decodeExtendedWindowStyle(int windowStyle);

    std::wstring toWideString(const std::string& source);

    std::string fromWideString(const std::wstring& source);

    VL_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags);


}