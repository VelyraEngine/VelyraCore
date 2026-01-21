#include "../../../Pch.hpp"

#include "Glfw3PlatformContext.hpp"

namespace Velyra::Core {

    Glfw3PlatformContext::Glfw3PlatformContext(const ContextDesc& desc, GLFWwindow* window):
    GLPlatformContext(VL_GL_PLATFORM_GLFW3),
    m_Window(window) {
    }

    Glfw3PlatformContext::~Glfw3PlatformContext() {
    }

    void Glfw3PlatformContext::setVerticalSynchronisation(const bool enable) {
        // Glfw3 does not provide a way to query VSync state, so we store it ourselves
        m_VSyncEnabled = enable;
        glfwSwapInterval(enable ? 1 : 0);
    }

    bool Glfw3PlatformContext::isVerticalSynchronisationEnabled() const {
        return m_VSyncEnabled;
    }

    void Glfw3PlatformContext::swapBuffers() {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot swap buffers")

        glfwSwapBuffers(m_Window);
    }

    void Glfw3PlatformContext::makeCurrent() {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot swap buffers")

        glfwMakeContextCurrent(m_Window);
    }

    void Glfw3PlatformContext::initPlatformImGui(const ImGuiContextDesc& /*desc*/) {
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    }

    void Glfw3PlatformContext::terminatePlatformImGui() {
        ImGui_ImplGlfw_Shutdown();
    }

    void Glfw3PlatformContext::onPlatformImGuiBegin() {
        ImGui_ImplGlfw_NewFrame();
    }

    void Glfw3PlatformContext::onPlatformImGuiEnd() {
        // Nothing to do here for Glfw3
    }

    U32 Glfw3PlatformContext::getClientWidth() const {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot get client width")

        int width;
        int height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return static_cast<U32>(width);
    }

    U32 Glfw3PlatformContext::getClientHeight() const {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot get client height")

        int width;
        int height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return static_cast<U32>(height);
    }

    void Glfw3PlatformContext::setWindowHints(const ContextDesc &desc) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, desc.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, desc.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, desc.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, desc.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, desc.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, desc.stencilBits);
#if defined(VL_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    }
}
