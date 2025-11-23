#pragma once

#include <VelyraUtils/VelyraEnum.hpp>
#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraCore/Window/KeyboardButtons.hpp>
#include <VelyraCore/Window/MouseButtons.hpp>

VL_ENUM(VL_EVENT_CLASS, int,
    VL_EVENT_CLASS_WINDOW   = 0x10,  // 0001 0000
    VL_EVENT_CLASS_MOUSE    = 0x20,  // 0010 0000
    VL_EVENT_CLASS_KEYBOARD = 0x40   // 0100 0000
    );

VL_ENUM(VL_EVENT_TYPE, int,
    VL_EVENT_NONE =                     0x00,

    VL_EVENT_WINDOW_MOVED =             0x11,
    VL_EVENT_WINDOW_RESIZED =           0x12,
    VL_EVENT_WINDOW_OPENED =            0x13,
    VL_EVENT_WINDOW_CLOSED =            0x14,
    VL_EVENT_WINDOW_LOST_FOCUS =        0x15,
    VL_EVENT_WINDOW_GAINED_FOCUS =      0x16,
    VL_EVENT_WINDOW_FILE_DROPPED =      0x17,
    VL_EVENT_WINDOW_REFRESHED =         0x18,

    VL_EVENT_MOUSE_MOVED =              0x21,
    VL_EVENT_MOUSE_SCROLLED =           0x22,
    VL_EVENT_MOUSE_BUTTON_PRESSED =     0x23,
    VL_EVENT_MOUSE_BUTTON_RELEASED =    0x24,
    VL_EVENT_MOUSE_ENTERED =            0x25,
    VL_EVENT_MOUSE_LEFT =               0x26,
    VL_EVENT_RAW_MOUSE_MOVED =          0x27,

    VL_EVENT_KEYBOARD_KEY_TYPED =       0x41,
    VL_EVENT_KEYBOARD_KEY_PRESSED =     0x42,
    VL_EVENT_KEYBOARD_KEY_RELEASED      = 0x43,
    VL_EVENT_RAW_KEYBOARD_KEY_PRESSED   = 0x45,
    VL_EVENT_RAW_KEYBOARD_KEY_RELEASED  = 0x46
    );

namespace Velyra::Core {

    struct VL_API Event {
        const VL_EVENT_CLASS eventClass = VL_EVENT_CLASS_MAX_VALUE;
        const VL_EVENT_TYPE type = VL_EVENT_TYPE_MAX_VALUE;
        const union {
            I32 xParam, windowPosX, windowWidth, mousePosX, mouseScrollX, mouseDeltaX, mouseButton, keyCode;
        };
        const union {
            I32 yParam, windowPosY, windowHeight, mousePosY, mouseScrollY, mouseDeltaY;
        };
        const char keyTyped = 0;

        Event() = default;

        Event(VL_EVENT_CLASS eventClass, VL_EVENT_TYPE type, I32 val1 = 0, I32 val2 = 0, char keyTyped = 0):
        eventClass(eventClass), type(type), xParam(val1), yParam(val2), keyTyped(keyTyped) {}
    };
    
}