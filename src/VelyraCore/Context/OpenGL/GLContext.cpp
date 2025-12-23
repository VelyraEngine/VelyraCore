#include "../../Pch.hpp"

#include "GLContext.hpp"
#include "../../../Logging/LoggerNames.hpp"
#include "../../../Logging/GLLogging.hpp"
#include "GLDevice.hpp"

namespace Velyra::Core {

    U64 GLContext::m_ContextCount = 0;

    GLContext::GLContext(const ContextDesc &desc, UP<GLPlatformContext> platformContext):
    Context(desc.api),
    m_PlatformContext(std::move(platformContext)),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        initGlad();
        m_Device = createUP<GLDevice>();
    }

    GLContext::~GLContext() {
        terminateGlad();

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

        VL_CORE_OPENGL_CALL(m_PlatformContext->swapBuffers())
    }

    void GLContext::makeCurrent() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->makeCurrent();
    }

    void GLContext::createImGuiContext(const ImGuiContextDesc &desc) {
        Context::createImGuiContext(desc);
        m_PlatformContext->initPlatformImGui(desc);
        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void GLContext::DestroyImGuiContext() {
        ImGui_ImplOpenGL3_Shutdown();
        m_PlatformContext->terminatePlatformImGui();
        Context::DestroyImGuiContext();
    }

    void GLContext::onImGuiBegin() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(!m_ImGuiRendering, "ImGui rendering already started!")

        ImGui_ImplOpenGL3_NewFrame();
        m_PlatformContext->onPlatformImGuiBegin();
        ImGui::NewFrame();

        m_ImGuiRendering = true;
    }

    void GLContext::onImGuiEnd() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(m_ImGuiRendering, "ImGui rendering already started!")

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_PlatformContext->onPlatformImGuiEnd();

        m_ImGuiRendering = false;
    }

    U32 GLContext::getClientWidth() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientWidth();
    }

    U32 GLContext::getClientHeight() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientHeight();
    }

    void GLContext::initGlad() const {
        if (m_ContextCount == 0) {
            const int version = gladLoadGL();
            if (version == 0) {
                SPDLOG_LOGGER_ERROR(m_Logger, "Failed to initialize OpenGL context");
                return;
            }
            SPDLOG_LOGGER_INFO(m_Logger, "Initialized OpenGL context with version {}.{}", GLVersion.major, GLVersion.minor);

#if defined(VL_DEBUG)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            const auto callback = static_cast<GLDEBUGPROC>(openGLMessageCallback);
            glDebugMessageCallback(callback, nullptr);
#endif
        }
        m_ContextCount++;
    }

    void GLContext::terminateGlad() {
        if (m_ContextCount == 0) {
            return;
        }
        m_ContextCount--;
        // Apparently, newer versions of glad do not have an unload function -> TODO: Investigate
    }


}