#include "../Pch.hpp"

#include <VelyraCore/Window/Window.hpp>
#include <VelyraCore/Context/Context.hpp>

namespace Velyra::Core {

    Window::~Window() = default;

    const UP<Context> &Window::getContext() const {
        return m_Context;
    }

    void Window::destroyContext() {
        m_Context.reset();
    }
}
