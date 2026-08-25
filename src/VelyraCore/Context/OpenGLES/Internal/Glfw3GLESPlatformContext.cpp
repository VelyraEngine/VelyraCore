#include "../../../Pch.hpp"

#include "Glfw3GLESPlatformContext.hpp"

#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core::GLES {

    U64 Glfw3GLESPlatformContext::m_ContextCount = 0;
    
    Glfw3GLESPlatformContext::Glfw3GLESPlatformContext(const ContextDesc& /*desc*/, GLFWwindow* window):
    GLESPlatformContext(VL_GLES_PLATFORM_GLFW3),
    m_Window(window) {
        glfwMakeContextCurrent(m_Window);
    }

    Glfw3GLESPlatformContext::~Glfw3GLESPlatformContext() = default;

    void Glfw3GLESPlatformContext::setVerticalSynchronisation(const bool enable) {
        // Glfw3 does not provide a way to query VSync state, so we store it ourselves
        m_VSyncEnabled = enable;
        glfwSwapInterval(enable ? 1 : 0);
    }

    bool Glfw3GLESPlatformContext::isVerticalSynchronisationEnabled() const {
        return m_VSyncEnabled;
    }

    void Glfw3GLESPlatformContext::swapBuffers() {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot swap buffers")

        glfwSwapBuffers(m_Window);
    }

    void Glfw3GLESPlatformContext::makeCurrent() {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot swap buffers")

        glfwMakeContextCurrent(m_Window);
    }

    void Glfw3GLESPlatformContext::initPlatformImGui(const ImGuiContextDesc& desc) {
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        m_ImGuiDesc = desc;
    }

    void Glfw3GLESPlatformContext::terminatePlatformImGui() {
        ImGui_ImplGlfw_Shutdown();
    }

    void Glfw3GLESPlatformContext::onPlatformImGuiBegin() {
        ImGui_ImplGlfw_NewFrame();
    }

    void Glfw3GLESPlatformContext::onPlatformImGuiEnd() {
        // Since OpenGL works with global functions, we have to switch contexts for every window ImGui creates
        // So we store our context, let ImGui do its thing and then restore our context
        if (m_ImGuiDesc.useViewports) {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }

    U32 Glfw3GLESPlatformContext::getClientWidth() const {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot get client width")

        int width;
        int height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return static_cast<U32>(width);
    }

    U32 Glfw3GLESPlatformContext::getClientHeight() const {
        VL_PRECONDITION(m_Window != nullptr, "GLFW window is null, cannot get client height")

        int width;
        int height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return static_cast<U32>(height);
    }

    void Glfw3GLESPlatformContext::initGlad() const {
        if (m_ContextCount == 0) {
            const auto logger = Utils::getLogger(VL_LOGGER_GL_ES);

            const int version = gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            if (version == 0) {
                SPDLOG_LOGGER_ERROR(logger, "Failed to initialize OpenGL ES context");
                return;
            }
            SPDLOG_LOGGER_INFO(logger, "Initialized OpenGL ES context with version {}.{}", GLVersion.major, GLVersion.minor);
        }
        m_ContextCount++;
    }

    void Glfw3GLESPlatformContext::terminateGlad() const {
        if (m_ContextCount == 0) {
            return;
        }
        m_ContextCount--;
    }

    void Glfw3GLESPlatformContext::setWindowHints(const ContextDesc &desc) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_RED_BITS, desc.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, desc.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, desc.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, desc.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, desc.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, desc.stencilBits);
    }

}
