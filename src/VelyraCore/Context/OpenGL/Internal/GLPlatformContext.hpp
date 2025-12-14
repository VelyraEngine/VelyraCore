#pragma once

#include <VelyraCore/Context/Context.hpp>

VL_ENUM(VL_GL_PLATFORM_API, int,
    VL_GL_PLATFORM_NONE,
    VL_GL_PLATFORM_WGL,
    VL_GL_PLATFORM_GLX,
    VL_GL_PLATFORM_EGL
);

namespace Velyra::Core {

    class GLPlatformContext {
    public:
        virtual ~GLPlatformContext() = default;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        virtual void swapBuffers() = 0;

        virtual void makeCurrent() = 0;

        [[nodiscard]] VL_GL_PLATFORM_API getType() const {
            return m_Type;
        }

    protected:
        explicit GLPlatformContext(const VL_GL_PLATFORM_API type): m_Type(type) {}

    protected:
        const VL_GL_PLATFORM_API m_Type;
    };
}

