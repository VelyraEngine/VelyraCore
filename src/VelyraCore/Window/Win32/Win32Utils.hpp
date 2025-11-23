#pragma once

namespace Velyra::Core {

    void setProcessDPIAware();

    DWORD decodeWindowStyle(int windowStyle);

    DWORD decodeExtendedWindowStyle(int windowStyle);

    std::wstring toWideString(const std::string& source);

    std::string fromWideString(const std::wstring& source);

    VL_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags);


}