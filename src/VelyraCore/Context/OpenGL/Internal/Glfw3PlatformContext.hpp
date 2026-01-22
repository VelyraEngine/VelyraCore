#pragma once

#include "GLPlatformContext.hpp"

namespace Velyra::Core {

    class Glfw3PlatformContext: public GLPlatformContext {
    public:
        Glfw3PlatformContext(const ContextDesc& desc, GLFWwindow* window);

        ~Glfw3PlatformContext() override;

        void setVerticalSynchronisation(bool enable) override;

        bool isVerticalSynchronisationEnabled() const override;

        void swapBuffers() override;

        void makeCurrent() override;

        void initPlatformImGui(const ImGuiContextDesc& desc) override;

        void terminatePlatformImGui() override;

        void onPlatformImGuiBegin() override;

        void onPlatformImGuiEnd() override;

        [[nodiscard]] U32 getClientWidth() const override;

        [[nodiscard]] U32 getClientHeight() const override;

        static void setWindowHints(const ContextDesc& desc);

    private:
        bool m_VSyncEnabled = false;
        GLFWwindow* m_Window = nullptr;

        ImGuiContextDesc m_ImGuiDesc{};
    };
}