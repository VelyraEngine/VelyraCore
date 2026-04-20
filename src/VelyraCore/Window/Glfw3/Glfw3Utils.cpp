#include "../../Pch.hpp"

#include "Glfw3Utils.hpp"
#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    Size Glfw3Instance::m_InstanceCount = 0;
    Utils::LogPtr Glfw3Instance::m_Logger = Utils::getLogger(VL_LOGGER_WINDOW);

    void Glfw3Instance::createInstance() {
        if (m_InstanceCount == 0) {
            setFlags();
            if (glfwInit() == GLFW_FALSE) {
                SPDLOG_LOGGER_CRITICAL(m_Logger, "Failed to initialize GLFW");
                throw std::runtime_error("Failed to initialize GLFW");
            }
            int major;
            int minor;
            int revision;
            glfwGetVersion(&major, &minor, &revision);
            SPDLOG_LOGGER_INFO(m_Logger, "GLFW3 initialised with version: {}.{}.{}", major, minor, revision);
        }
        m_InstanceCount++;
        SPDLOG_LOGGER_INFO(m_Logger, "GLFW current instance count {}", m_InstanceCount);
    }

    void Glfw3Instance::destroyInstance() {
        m_InstanceCount--;
        SPDLOG_LOGGER_INFO(m_Logger, "GLFW current instance count {}", m_InstanceCount);
        if (m_InstanceCount == 0) {
            glfwTerminate();
        }
    }

    Size Glfw3Instance::getInstanceCount() {
        return m_InstanceCount;
    }

    void Glfw3Instance::setFlags() {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }

    VL_KEYBOARD_KEY translateGlfwKey(const int key) {
        switch (key) {
            case GLFW_KEY_SPACE:        return VL_KEY_SPACE;
            case GLFW_KEY_APOSTROPHE:   return VL_KEY_APOSTROPHE;
            case GLFW_KEY_COMMA:        return VL_KEY_COMMA;
            case GLFW_KEY_MINUS:        return VL_KEY_MINUS;
            case GLFW_KEY_PERIOD:       return VL_KEY_PERIOD;
            case GLFW_KEY_SLASH:        return VL_KEY_SLASH;
            case GLFW_KEY_0:            return VL_KEY_0;
            case GLFW_KEY_1:            return VL_KEY_1;
            case GLFW_KEY_2:            return VL_KEY_2;
            case GLFW_KEY_3:            return VL_KEY_3;
            case GLFW_KEY_4:            return VL_KEY_4;
            case GLFW_KEY_5:            return VL_KEY_5;
            case GLFW_KEY_6:            return VL_KEY_6;
            case GLFW_KEY_7:            return VL_KEY_7;
            case GLFW_KEY_8:            return VL_KEY_8;
            case GLFW_KEY_9:            return VL_KEY_9;
            case GLFW_KEY_SEMICOLON:    return VL_KEY_SEMICOLON;
            case GLFW_KEY_EQUAL:        return VL_KEY_EQUAL;
            case GLFW_KEY_A:            return VL_KEY_A;
            case GLFW_KEY_B:            return VL_KEY_B;
            case GLFW_KEY_C:            return VL_KEY_C;
            case GLFW_KEY_D:            return VL_KEY_D;
            case GLFW_KEY_E:            return VL_KEY_E;
            case GLFW_KEY_F:            return VL_KEY_F;
            case GLFW_KEY_G:            return VL_KEY_G;
            case GLFW_KEY_H:            return VL_KEY_H;
            case GLFW_KEY_I:            return VL_KEY_I;
            case GLFW_KEY_J:            return VL_KEY_J;
            case GLFW_KEY_K:            return VL_KEY_K;
            case GLFW_KEY_L:            return VL_KEY_L;
            case GLFW_KEY_M:            return VL_KEY_M;
            case GLFW_KEY_N:            return VL_KEY_N;
            case GLFW_KEY_O:            return VL_KEY_O;
            case GLFW_KEY_P:            return VL_KEY_P;
            case GLFW_KEY_Q:            return VL_KEY_Q;
            case GLFW_KEY_R:            return VL_KEY_R;
            case GLFW_KEY_S:            return VL_KEY_S;
            case GLFW_KEY_T:            return VL_KEY_T;
            case GLFW_KEY_U:            return VL_KEY_U;
            case GLFW_KEY_V:            return VL_KEY_V;
            case GLFW_KEY_W:            return VL_KEY_W;
            case GLFW_KEY_X:            return VL_KEY_X;
            case GLFW_KEY_Y:            return VL_KEY_Y;
            case GLFW_KEY_Z:            return VL_KEY_Z;
            case GLFW_KEY_LEFT_BRACKET: return VL_KEY_LEFT_BRACKET;
            case GLFW_KEY_BACKSLASH:    return VL_KEY_BACKSLASH;
            case GLFW_KEY_RIGHT_BRACKET:return VL_KEY_RIGHT_BRACKET;
            case GLFW_KEY_GRAVE_ACCENT: return VL_KEY_GRAVE_ACCENT;
            case GLFW_KEY_WORLD_1:      return VL_KEY_WORLD_1;
            case GLFW_KEY_WORLD_2:      return VL_KEY_WORLD_2;
            case GLFW_KEY_ESCAPE:       return VL_KEY_ESCAPE;
            case GLFW_KEY_ENTER:        return VL_KEY_ENTER;
            case GLFW_KEY_TAB:          return VL_KEY_TAB;
            case GLFW_KEY_BACKSPACE:    return VL_KEY_BACKSPACE;
            case GLFW_KEY_INSERT:       return VL_KEY_INSERT;
            case GLFW_KEY_DELETE:       return VL_KEY_DELETE;
            case GLFW_KEY_RIGHT:        return VL_KEY_RIGHT;
            case GLFW_KEY_LEFT:         return VL_KEY_LEFT;
            case GLFW_KEY_DOWN:         return VL_KEY_DOWN;
            case GLFW_KEY_UP:           return VL_KEY_UP;
            case GLFW_KEY_PAGE_UP:      return VL_KEY_PAGE_UP;
            case GLFW_KEY_PAGE_DOWN:    return VL_KEY_PAGE_DOWN;
            case GLFW_KEY_HOME:         return VL_KEY_HOME;
            case GLFW_KEY_END:          return VL_KEY_END;
            case GLFW_KEY_CAPS_LOCK:    return VL_KEY_CAPS_LOCK;
            case GLFW_KEY_SCROLL_LOCK:  return VL_KEY_SCROLL_LOCK;
            case GLFW_KEY_NUM_LOCK:     return VL_KEY_NUM_LOCK;
            case GLFW_KEY_PRINT_SCREEN: return VL_KEY_PRINT_SCREEN;
            case GLFW_KEY_PAUSE:        return VL_KEY_PAUSE;
            case GLFW_KEY_F1:           return VL_KEY_F1;
            case GLFW_KEY_F2:           return VL_KEY_F2;
            case GLFW_KEY_F3:           return VL_KEY_F3;
            case GLFW_KEY_F4:           return VL_KEY_F4;
            case GLFW_KEY_F5:           return VL_KEY_F5;
            case GLFW_KEY_F6:           return VL_KEY_F6;
            case GLFW_KEY_F7:           return VL_KEY_F7;
            case GLFW_KEY_F8:           return VL_KEY_F8;
            case GLFW_KEY_F9:           return VL_KEY_F9;
            case GLFW_KEY_F10:          return VL_KEY_F10;
            case GLFW_KEY_F11:          return VL_KEY_F11;
            case GLFW_KEY_F12:          return VL_KEY_F12;
            case GLFW_KEY_F13:          return VL_KEY_F13;
            case GLFW_KEY_F14:          return VL_KEY_F14;
            case GLFW_KEY_F15:          return VL_KEY_F15;
            case GLFW_KEY_F16:          return VL_KEY_F16;
            case GLFW_KEY_F17:          return VL_KEY_F17;
            case GLFW_KEY_F18:          return VL_KEY_F18;
            case GLFW_KEY_F19:          return VL_KEY_F19;
            case GLFW_KEY_F20:          return VL_KEY_F20;
            case GLFW_KEY_F21:          return VL_KEY_F21;
            case GLFW_KEY_F22:          return VL_KEY_F22;
            case GLFW_KEY_F23:          return VL_KEY_F23;
            case GLFW_KEY_F24:          return VL_KEY_F24;
            case GLFW_KEY_KP_0:         return VL_KEY_NUMPAD_0;
            case GLFW_KEY_KP_1:         return VL_KEY_NUMPAD_1;
            case GLFW_KEY_KP_2:         return VL_KEY_NUMPAD_2;
            case GLFW_KEY_KP_3:         return VL_KEY_NUMPAD_3;
            case GLFW_KEY_KP_4:         return VL_KEY_NUMPAD_4;
            case GLFW_KEY_KP_5:         return VL_KEY_NUMPAD_5;
            case GLFW_KEY_KP_6:         return VL_KEY_NUMPAD_6;
            case GLFW_KEY_KP_7:         return VL_KEY_NUMPAD_7;
            case GLFW_KEY_KP_8:         return VL_KEY_NUMPAD_8;
            case GLFW_KEY_KP_9:         return VL_KEY_NUMPAD_9;
            case GLFW_KEY_KP_DECIMAL:   return VL_KEY_KP_DECIMAL;
            case GLFW_KEY_KP_DIVIDE:    return VL_KEY_KP_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY:  return VL_KEY_KP_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT:  return VL_KEY_KP_SUBTRACT;
            case GLFW_KEY_KP_ADD:       return VL_KEY_KP_ADD;
            case GLFW_KEY_KP_ENTER:     return VL_KEY_KP_ENTER;
            case GLFW_KEY_KP_EQUAL:     return VL_KEY_KP_EQUAL;
            case GLFW_KEY_LEFT_SHIFT:   return VL_KEY_LEFT_SHIFT;
            case GLFW_KEY_LEFT_CONTROL: return VL_KEY_LEFT_CONTROL;
            case GLFW_KEY_LEFT_ALT:     return VL_KEY_LEFT_ALT;
            case GLFW_KEY_LEFT_SUPER:   return VL_KEY_LEFT_SUPER;
            case GLFW_KEY_RIGHT_SHIFT:  return VL_KEY_RIGHT_SHIFT;
            case GLFW_KEY_RIGHT_CONTROL:return VL_KEY_RIGHT_CONTROL;
            case GLFW_KEY_RIGHT_ALT:    return VL_KEY_RIGHT_ALT;
            case GLFW_KEY_RIGHT_SUPER:  return VL_KEY_RIGHT_SUPER;
            case GLFW_KEY_MENU:         return VL_KEY_MENU;
            default:                    return VL_KEY_NONE;
        }
    }

    VL_MOUSE_BUTTON translateGlfwMouseButton(const int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:    return VL_MOUSE_BUTTON_LEFT;
            case GLFW_MOUSE_BUTTON_RIGHT:   return VL_MOUSE_BUTTON_RIGHT;
            case GLFW_MOUSE_BUTTON_3:       return VL_MOUSE_BUTTON_MIDDLE;
            case GLFW_MOUSE_BUTTON_4:       return VL_MOUSE_BUTTON_X1;
            case GLFW_MOUSE_BUTTON_5:       return VL_MOUSE_BUTTON_X2;
            default:                        return VL_MOUSE_BUTTON_NONE;
        }
    }
}
