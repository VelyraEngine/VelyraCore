#include "GLContext.hpp"

namespace Velyra::Core {

    GLContext::GLContext(const ContextDesc &desc, UP<GLPlatformContext> platformContext):
    Context(desc.api),
    m_PlatformContext(std::move(platformContext)){

    }

    GLContext::~GLContext() {

    }

    void GLContext::setVerticalSynchronisation(const bool enable) {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->setVerticalSynchronisation(enable);
    }

    bool GLContext::isVerticalSynchronisationEnabled() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->isVerticalSynchronisationEnabled();
    }

    void GLContext::swapBuffers() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->swapBuffers();
    }

    void GLContext::makeCurrent() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->makeCurrent();
    }

}