#include <VelyraCore/Window/WindowFactory.hpp>

#if defined(VL_PLATFORM_WINDOWS)

    #include "Win32/Win32Window.hpp"

#elif defined(VL_PLATFORM_LINUX)

    #include "Glfw3/Glfw3Window.hpp"

#endif

namespace Velyra::Core {

#if defined(VL_PLATFORM_WINDOWS)

    UP<Window> WindowFactory::createWindow(const WindowDesc &desc) {
        return createUP<Win32Window>(desc);
    }

#elif defined(VL_PLATFORM_LINUX)

    UP<Window> WindowFactory::createWindow(const WindowDesc &desc) {
        return createUP<Glfw3Window>(desc);
    }

#endif

}