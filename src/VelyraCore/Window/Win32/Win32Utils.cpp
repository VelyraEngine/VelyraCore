#include "../../Pch.hpp"

#include <VelyraCore/Window/Window.hpp>
#include "Win32Utils.hpp"
#include "../../LoggerNames.hpp"

namespace Velyra::Core {

    enum ProcessDpiAwareness {
        ProcessDpiUnaware         = 0,
        ProcessSystemDpiAware     = 1,
        ProcessPerMonitorDpiAware = 2
    };

    void setProcessDPIAware() {
        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_WINDOW);

        HINSTANCE shCoreDll = LoadLibraryW(L"Shcore.dll");
        if (shCoreDll){
            using SetProcessDPIAwareFuncType = HRESULT(WINAPI*)(int);
            auto setDpiAwareness  = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));
            if (setDpiAwareness ){
                if (setDpiAwareness(ProcessSystemDpiAware) == E_INVALIDARG){
                    SPDLOG_LOGGER_WARN(logger, "Failed to set process DPI awareness using shCore.dll libary, falling back on user32.dll");
                }
                else{
                    FreeLibrary(shCoreDll);
                    return;
                }
            }
            FreeLibrary(shCoreDll);
        }
        else {
            SPDLOG_LOGGER_WARN(logger, "Failed to load shcore.dll library, falling back on user32.dll");
        }
        // when setting DPI awareness using shcore.dll failed, fall back and use user32.dll and try again
        HINSTANCE user32Dll = LoadLibraryW(L"user32.dll");
        if (user32Dll){
            using SetProcessDPIAwareFuncType = BOOL(WINAPI*)();
            auto setDpiAwareness = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32Dll, "SetProcessDPIAware"));
            // First check if the function got loaded correctly
            if (setDpiAwareness){
                if (!setDpiAwareness()){
                    SPDLOG_LOGGER_WARN(logger, "Failed to set process DPI awareness");
                }
            }
            FreeLibrary(user32Dll);
        }
    }

    DWORD decodeWindowStyle(const int windowStyle){
        DWORD style = WS_VISIBLE;
        if (windowStyle == VL_WINDOW_STYLE_DEFAULT){
            style |=  WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION;
            return style;
        }
        if (windowStyle & VL_WINDOW_STYLE_POPUP){
            style |= WS_POPUP;
            return style;
        }
        if (windowStyle & VL_WINDOW_STYLE_CLOSE){
            style |= WS_SYSMENU;
        }
        if (windowStyle & VL_WINDOW_STYLE_TITLEBAR){
            style |= WS_CAPTION;
        }
        if (windowStyle & VL_WINDOW_STYLE_RESIZE){
            style |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        }
        return style;
    }

    DWORD decodeExtendedWindowStyle(const int windowStyle){
        DWORD style = 0;
        if (windowStyle == VL_WINDOW_STYLE_DEFAULT){
            style |= WS_EX_ACCEPTFILES;
            return style;
        }
        if (windowStyle & VL_WINDOW_STYLE_FILE_DROP){
            style |= WS_EX_ACCEPTFILES;
        }
        if (windowStyle & VL_WINDOW_STYLE_SUNKEN_EDGE){
            style |= WS_EX_CLIENTEDGE;
        }
        if (windowStyle & VL_WINDOW_STYLE_RAISED_EDGE){
            style |= WS_EX_WINDOWEDGE;
        }
        return style;
    }

    std::wstring toWideString(const std::string& source) {
        int len = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
        if (len == 0) {
            return L"";
        }
        const auto buf = createUP<wchar_t[]>(len);
        MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, buf.get(), len);
        return std::wstring(buf.get());
    }

    std::string fromWideString(const std::wstring& source) {
        int len = WideCharToMultiByte(CP_ACP, 0, source.c_str(), source.size(), nullptr, 0, nullptr, nullptr);
        if (len == 0) {
            return "";
        }
        const auto buf = createUP<char[]>(len);
        WideCharToMultiByte(CP_ACP, 0, source.c_str(), source.size(), buf.get(), len, nullptr, nullptr);
        return std::string(buf.get());
    }

    VL_KEYBOARD_KEY convertVirtualKey(const WPARAM key, const LPARAM flags) {
        switch (key) {
            case VK_SHIFT: {
                const U32 lshift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
                const auto scancode = static_cast<U32>((flags & (0xFF << 16)) >> 16);
                if (scancode == lshift){
                    return VL_KEY_LEFT_SHIFT;
                }
                return VL_KEY_RIGHT_SHIFT;
            }
            case VK_MENU: {
                if (HIWORD(flags) & KF_EXTENDED){
                    return VL_KEY_RIGHT_ALT;
                }
                return VL_KEY_LEFT_ALT;
            }
            case VK_CONTROL: {
                if (HIWORD(flags) & KF_EXTENDED){
                    return VL_KEY_RIGHT_CONTROL;
                }
                else{
                    return VL_KEY_LEFT_CONTROL;
                }
            }
            case VK_RCONTROL:   return VL_KEY_RIGHT_CONTROL;
            case VK_LCONTROL:   return VL_KEY_LEFT_CONTROL;
            case VK_OEM_1:      return VL_KEY_SEMICOLON;
            case VK_OEM_2:      return VL_KEY_SLASH;
            case VK_OEM_PLUS:   return VL_KEY_EQUAL;
            case VK_OEM_MINUS:  return VL_KEY_MINUS;
            case VK_OEM_4:      return VL_KEY_LEFT_BRACKET;
            case VK_OEM_6:      return VL_KEY_RIGHT_BRACKET;
            case VK_OEM_COMMA:  return VL_KEY_COMMA;
            case VK_OEM_PERIOD: return VL_KEY_PERIOD;
            case VK_OEM_7:      return VL_KEY_GRAVE_ACCENT;
            case VK_OEM_5:      return VL_KEY_BACKSLASH;
            case VK_NUMLOCK:    return VL_KEY_NUM_LOCK;
            case VK_SCROLL:     return VL_KEY_SCROLL_LOCK;
            case VK_CAPITAL:    return VL_KEY_CAPS_LOCK;
            case VK_ESCAPE:     return VL_KEY_ESCAPE;
            case VK_SPACE:      return VL_KEY_SPACE;
            case VK_SNAPSHOT:   return VL_KEY_PRINT_SCREEN;
            case VK_RETURN:     return VL_KEY_ENTER;
            case VK_BACK:       return VL_KEY_BACKSPACE;
            case VK_TAB:        return VL_KEY_TAB;
            case VK_PRIOR:      return VL_KEY_PAGE_UP;
            case VK_NEXT:       return VL_KEY_PAGE_DOWN;
            case VK_END:        return VL_KEY_END;
            case VK_HOME:       return VL_KEY_HOME;
            case VK_INSERT:     return VL_KEY_INSERT;
            case VK_DELETE:     return VL_KEY_DELETE;
            case VK_ADD:        return VL_KEY_KP_ADD;
            case VK_SUBTRACT:   return VL_KEY_KP_SUBTRACT;
            case VK_MULTIPLY:   return VL_KEY_KP_MULTIPLY;
            case VK_DIVIDE:     return VL_KEY_KP_DIVIDE;
            case VK_PAUSE:      return VL_KEY_PAUSE;
            case VK_F1:         return VL_KEY_F1;
            case VK_F2:         return VL_KEY_F2;
            case VK_F3:         return VL_KEY_F3;
            case VK_F4:         return VL_KEY_F4;
            case VK_F5:         return VL_KEY_F5;
            case VK_F6:         return VL_KEY_F6;
            case VK_F7:         return VL_KEY_F7;
            case VK_F8:         return VL_KEY_F8;
            case VK_F9:         return VL_KEY_F9;
            case VK_F10:        return VL_KEY_F10;
            case VK_F11:        return VL_KEY_F11;
            case VK_F12:        return VL_KEY_F12;
            case VK_F13:        return VL_KEY_F13;
            case VK_F14:        return VL_KEY_F14;
            case VK_F15:        return VL_KEY_F15;
            case VK_F16:        return VL_KEY_F16;
            case VK_F17:        return VL_KEY_F17;
            case VK_F18:        return VL_KEY_F18;
            case VK_F19:        return VL_KEY_F19;
            case VK_F20:        return VL_KEY_F20;
            case VK_F21:        return VL_KEY_F21;
            case VK_F22:        return VL_KEY_F22;
            case VK_F23:        return VL_KEY_F23;
            case VK_F24:        return VL_KEY_F24;
            case VK_LEFT:       return VL_KEY_LEFT;
            case VK_RIGHT:      return VL_KEY_RIGHT;
            case VK_UP:         return VL_KEY_UP;
            case VK_DOWN:       return VL_KEY_DOWN;
            case VK_NUMPAD0:    return VL_KEY_NUMPAD_0;
            case VK_NUMPAD1:    return VL_KEY_NUMPAD_1;
            case VK_NUMPAD2:    return VL_KEY_NUMPAD_2;
            case VK_NUMPAD3:    return VL_KEY_NUMPAD_3;
            case VK_NUMPAD4:    return VL_KEY_NUMPAD_4;
            case VK_NUMPAD5:    return VL_KEY_NUMPAD_5;
            case VK_NUMPAD6:    return VL_KEY_NUMPAD_6;
            case VK_NUMPAD7:    return VL_KEY_NUMPAD_7;
            case VK_NUMPAD8:    return VL_KEY_NUMPAD_8;
            case VK_NUMPAD9:    return VL_KEY_NUMPAD_9;
            case 'Q':           return VL_KEY_Q;
            case 'W':           return VL_KEY_W;
            case 'E':           return VL_KEY_E;
            case 'R':           return VL_KEY_R;
            case 'T':           return VL_KEY_T;
            case 'Y':           return VL_KEY_Y;
            case 'U':           return VL_KEY_U;
            case 'I':           return VL_KEY_I;
            case 'O':           return VL_KEY_O;
            case 'P':           return VL_KEY_P;
            case 'A':           return VL_KEY_A;
            case 'S':           return VL_KEY_S;
            case 'D':           return VL_KEY_D;
            case 'F':           return VL_KEY_F;
            case 'G':           return VL_KEY_G;
            case 'H':           return VL_KEY_H;
            case 'J':           return VL_KEY_J;
            case 'K':           return VL_KEY_K;
            case 'L':           return VL_KEY_L;
            case 'Z':           return VL_KEY_Z;
            case 'X':           return VL_KEY_X;
            case 'C':           return VL_KEY_C;
            case 'V':           return VL_KEY_V;
            case 'B':           return VL_KEY_B;
            case 'N':           return VL_KEY_N;
            case 'M':           return VL_KEY_M;
            case '0':           return VL_KEY_0;
            case '1':           return VL_KEY_1;
            case '2':           return VL_KEY_2;
            case '3':           return VL_KEY_3;
            case '4':           return VL_KEY_4;
            case '5':           return VL_KEY_5;
            case '6':           return VL_KEY_6;
            case '7':           return VL_KEY_7;
            case '8':           return VL_KEY_8;
            case '9':           return VL_KEY_9;
            default:            return VL_KEY_NONE;
        }
    }
}