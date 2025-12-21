#include "../Pch.hpp"

#include "GLLogging.hpp"
#include "LoggerNames.hpp"

namespace Velyra::Core {

    void formatOpenGLError(unsigned int error) {
        std::string errorMessage;
        switch (error) {
            case GL_INVALID_ENUM:                   errorMessage = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  errorMessage = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              errorMessage = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 errorMessage = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                errorMessage = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  errorMessage = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  errorMessage = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                                errorMessage = "UNKNOWN_ERROR"; break;
        }

        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
        SPDLOG_LOGGER_ERROR(logger, "OpenGL Error: {} ({})", errorMessage, error);
    }

    void clearOpenGLErrors() {
        while (glGetError() != GL_NO_ERROR) {
        }
    }

    void logOpenGLErrors() {
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            formatOpenGLError(error);
        }
    }

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
}