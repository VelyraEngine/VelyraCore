#pragma once

#include "Internal/GLPlatformContext.hpp"

namespace Velyra::Core {

    class GLContext: public Context {
    public:
        GLContext(const ContextDesc& desc, UP<GLPlatformContext> platformContext);

        ~GLContext() override;

        void setVerticalSynchronisation(bool enable) override;

        [[nodiscard]] bool isVerticalSynchronisationEnabled() const override;

        void swapBuffers() override;

        void makeCurrent() override;

        void initImGui(const ImGuiDesc& desc) override;

        void terminateImGui() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:

        void initGlad() const;

        void terminateGlad();

    private:
        static U64 m_ContextCount;

        UP<GLPlatformContext> m_PlatformContext;
        Utils::LogPtr m_Logger;
    };
}