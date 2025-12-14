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
}