#include <iostream>
#include "EventProcedure.hpp"

namespace Velyra::SandBox {

    EventProcedure::EventProcedure(): IProcedure() {

    }

    void EventProcedure::onEvent(DataContainer&, const Core::Event &event) {
        switch (event.type){
            case VL_EVENT_WINDOW_CLOSED:
                std::cout << "Window closed event" << std::endl;
                break;
            case VL_EVENT_WINDOW_RESIZED:
                std::cout << "Window resized event: " << event.windowWidth << "x" << event.windowHeight << std::endl;
                break;
            case VL_EVENT_WINDOW_MOVED:
                std::cout << "Window moved event: x: " << event.windowPosX << " y:" << event.windowPosY << std::endl;
                break;
            case VL_EVENT_WINDOW_GAINED_FOCUS:
                std::cout << "Window focus gained event" << std::endl;
                break;
            case VL_EVENT_WINDOW_LOST_FOCUS:
                std::cout << "Window focus lost event" << std::endl;
                break;
            case VL_EVENT_MOUSE_ENTERED:
                std::cout << "Window mouse entered event" << std::endl;
                break;
            case VL_EVENT_MOUSE_LEFT:
                std::cout << "Window mouse left event" << std::endl;
                break;
            case VL_EVENT_MOUSE_MOVED:
                std::cout << "Window mouse moved event: x: " << event.mousePosX << " y:" << event.mousePosY << std::endl;
                break;
            case VL_EVENT_MOUSE_SCROLLED:
                std::cout << "Window mouse scrolled event: X: " << event.mouseScrollX << " y:" << event.mouseScrollY << std::endl;
                break;
            case VL_EVENT_MOUSE_BUTTON_PRESSED:
                std::cout << "Window mouse button pressed event: " << event.mouseButton << std::endl;
                break;
            case VL_EVENT_MOUSE_BUTTON_RELEASED:
                std::cout << "Window mouse button released event: " << event.mouseButton << std::endl;
                break;
            case VL_EVENT_KEYBOARD_KEY_PRESSED:
                std::cout << "Window keyboard key pressed event: " << event.keyCode << std::endl;
                break;
            case VL_EVENT_KEYBOARD_KEY_RELEASED:
                std::cout << "Window keyboard key released event: " << event.keyCode << std::endl;
                break;
            case VL_EVENT_KEYBOARD_KEY_TYPED:
                std::cout << "Window keyboard key typed event: " << event.keyTyped << std::endl;
                break;
            case VL_EVENT_RAW_MOUSE_MOVED:
                std::cout << "Window raw mouse moved event: x: " << event.mousePosX << " y:" << event.mousePosY << std::endl;
                break;
            case VL_EVENT_RAW_KEYBOARD_KEY_PRESSED:
                std::cout << "Window raw keyboard key pressed event: " << event.keyCode << std::endl;
                break;
            case VL_EVENT_RAW_KEYBOARD_KEY_RELEASED:
                std::cout << "Window raw keyboard key released event: " << event.keyCode << std::endl;
                break;
            default:
                std::cout << "Unknown event" << std::endl;
                break;
        }
    }



}
