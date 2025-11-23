#include <VelyraCore/Window/WindowFactory.hpp>

#if defined(VL_PLATFORM_WINDOWS)
    #include "Win32/WindowWin32.hpp"
#endif

namespace Velyra::Core {

#if defined(VL_PLATFORM_WINDOWS)

    UP<Window> WindowFactory::createWindow(const WindowDesc &desc) {
        return createUP<WindowWin32>(desc);
    }

#endif

}