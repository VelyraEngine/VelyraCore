#pragma once

#include "GLESPlatformContext.hpp"

#include <VelyraCore/Context/Context.hpp>

namespace Velyra::Core::GLES {

    class Glfw3GLESPlatformContext: public GLESPlatformContext {
    public:
        Glfw3GLESPlatformContext(const ContextDesc& desc, GLFWwindow* window);
        ~Glfw3GLESPlatformContext() override;

        void setVerticalSynchronisation(bool enable) override;

        [[nodiscard]] bool isVerticalSynchronisationEnabled() const override;

        void swapBuffers() override;

        void makeCurrent() override;

        void initPlatformImGui(const ImGuiContextDesc& desc) override;

        void terminatePlatformImGui() override;

        void onPlatformImGuiBegin() override;

        void onPlatformImGuiEnd() override;

        [[nodiscard]] U32 getClientWidth() const override;

        [[nodiscard]] U32 getClientHeight() const override;

        void initGlad() const override;

        void terminateGlad() const override;

        static void setWindowHints(const ContextDesc& desc);

    private:
        static U64 m_ContextCount;

        bool m_VSyncEnabled = false;
        GLFWwindow* m_Window = nullptr;

        ImGuiContextDesc m_ImGuiDesc{};
    };

}