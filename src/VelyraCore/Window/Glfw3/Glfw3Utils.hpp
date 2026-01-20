#pragma once

#include <VelyraCore/Window/MouseButtons.hpp>
#include <VelyraCore/Window/KeyboardButtons.hpp>

namespace Velyra::Core {

    class Glfw3Instance {
    public:
        static void createInstance();

        static void destroyInstance();

        static Size getInstanceCount();

    private:

        static void setFlags();

    private:
        static Size m_InstanceCount;
    };

    VL_KEYBOARD_KEY translateGlfwKey(int key);

    VL_MOUSE_BUTTON translateGlfwMouseButton(int button);

}

