#pragma once

#include <VelyraUtils/VelyraEnum.hpp>
#include <VelyraUtils/Types/Types.hpp>

VL_ENUM(VL_GLES_PLATFORM_API, int,
    VL_GLES_PLATFORM_NONE,
    VL_GLES_PLATFORM_GLFW3
);

namespace Velyra::Core {
    struct ImGuiContextDesc;
}

namespace Velyra::Core::GLES {

    class GLESPlatformContext {
    public:
        virtual ~GLESPlatformContext() = default;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        [[nodiscard]] virtual bool isVerticalSynchronisationEnabled() const = 0;

        virtual void swapBuffers() = 0;

        virtual void makeCurrent() = 0;

        virtual void initPlatformImGui(const ImGuiContextDesc& desc) = 0;

        virtual void terminatePlatformImGui() = 0;

        virtual void onPlatformImGuiBegin() = 0;

        virtual void onPlatformImGuiEnd() = 0;

        // Glad initialization and termination functions are for OpenGLES platform dependant
        virtual void initGlad() const = 0;

        virtual void terminateGlad() const = 0;

        [[nodiscard]] virtual U32 getClientWidth() const = 0;

        [[nodiscard]] virtual U32 getClientHeight() const = 0;

        [[nodiscard]] VL_GLES_PLATFORM_API getType() const {
            return m_Type;
        }

    protected:
        explicit GLESPlatformContext(const VL_GLES_PLATFORM_API type): m_Type(type) {}

    protected:
        const VL_GLES_PLATFORM_API m_Type;
    };

}