#pragma once

#include <VelyraCore/Window/Window.hpp>

namespace Velyra::Core {

    class VL_API WindowFactory {
    public:
        static UP<Window> createWindow(const WindowDesc& desc = WindowDesc());
    };

}