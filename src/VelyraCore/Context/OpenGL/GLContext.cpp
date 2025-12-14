#include "../../Pch.hpp"

#include "GLContext.hpp"
#include "../../../Logging/LoggerNames.hpp"
#include "../../../Logging/GLLogging.hpp"

namespace Velyra::Core {

    U64 GLContext::m_ContextCount = 0;

    void openGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/,
        const GLchar* message, const void* /*userParam*/) {
        Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);

        std::string strType;
        std::string strSource;
        std::string strSeverity;
        std::string strMessage(message);
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               strType = "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: strType = "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  strType = "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         strType = "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         strType = "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              strType = "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          strType = "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           strType = "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               strType = "Other"; break;
            default:                                strType = "UNKNOWN TYPE"; break;
        }
        switch (source) {
            case GL_DEBUG_SOURCE_API:               strSource = "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     strSource = "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   strSource = "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:       strSource = "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:       strSource = "Application"; break;
            case GL_DEBUG_SOURCE_OTHER:             strSource = "Other"; break;
            default:                                strSource = "UNKNOWN SOURCE"; break;
        }
        switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:             strSeverity = "Low"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          strSeverity = "Medium"; break;
            case GL_DEBUG_SEVERITY_HIGH:            strSeverity = "High"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    strSeverity = "Notification"; break;
            default:                                strSeverity = "Unknown"; break;
        }

        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            SPDLOG_LOGGER_INFO(logger, "OpenGLMessageCallback: ID = {} | Type = {} | Source = {} | Severity = {} | Message = {}",
                id, strType, strSource, strSeverity, strMessage);
        }
        else {
            SPDLOG_LOGGER_WARN(logger, "OpenGLMessageCallback: ID = {} | Type = {} | Source = {} | Severity = {} | Message = {}",
                id, strType, strSource, strSeverity, strMessage);
        }
    }

    GLContext::GLContext(const ContextDesc &desc, UP<GLPlatformContext> platformContext):
    Context(desc.api),
    m_PlatformContext(std::move(platformContext)),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        initGlad();

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

    void GLContext::initImGui(const ImGuiDesc &desc) {
        Context::initImGui(desc);
        m_PlatformContext->initPlatformImGui(desc);
        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void GLContext::terminateImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        m_PlatformContext->terminatePlatformImGui();
        Context::terminateImGui();
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

    void GLContext::initGlad() {
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