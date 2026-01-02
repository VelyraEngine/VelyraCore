#include "../../../Pch.hpp"

#include "GLTranslations.hpp"
#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLenum getGLShaderType(const VL_SHADER_TYPE type) {
        switch (type) {
            case VL_SHADER_VERTEX:                  return GL_VERTEX_SHADER;
            case VL_SHADER_FRAGMENT:                return GL_FRAGMENT_SHADER;
            case VL_SHADER_GEOMETRY:                return GL_GEOMETRY_SHADER;
            case VL_SHADER_TESSELATION_CONTROL:     return GL_TESS_CONTROL_SHADER;
            case VL_SHADER_TESSELATION_EVALUATION:  return GL_TESS_EVALUATION_SHADER;
            case VL_SHADER_COMPUTE:                 return GL_COMPUTE_SHADER;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid shader type {} given to converter, return default: GL_VERTEX_SHADER", type);
                return GL_VERTEX_SHADER;
            }
        }
    }

    GLenum getGLBufferUsage(const VL_BUFFER_USAGE usage) {
        switch (usage) {
            case VL_BUFFER_USAGE_DEFAULT: return GL_DYNAMIC_DRAW;
            case VL_BUFFER_USAGE_DYNAMIC: return GL_DYNAMIC_DRAW;
            case VL_BUFFER_USAGE_STATIC:  return GL_STATIC_DRAW;
            case VL_BUFFER_USAGE_COPY:    return GL_STREAM_COPY;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid usage {} given to converter, return default: GL_DYNAMIC_DRAW", usage);
                return GL_STATIC_DRAW;
            }
        }
    }

}

