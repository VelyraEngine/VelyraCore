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
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid usage {} given to converter, return default: GL_DYNAMIC_DRAW", usage);
                return GL_STATIC_DRAW;
            }
        }
    }

    GLenum getGLDrawMode(const VL_DRAW_MODE mode) {
        switch (mode) {
            case VL_DRAW_POINTS:            return GL_POINTS;
            case VL_DRAW_LINES:             return GL_LINES;
            case VL_DRAW_LINES_STRIP:       return GL_LINE_STRIP;
            case VL_DRAW_TRIANGLES:         return GL_TRIANGLES;
            case VL_DRAW_TRIANGLE_STRIP:    return GL_TRIANGLE_STRIP;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid draw mode {} given to converter, return default: GL_TRIANGLES", mode);
                return GL_TRIANGLES;
            }
        }
    }

    GLenum getGLDataType(const VL_TYPE type) {
        switch (type) {
            case VL_INT8:       return GL_BYTE;
            case VL_UINT8:      return GL_UNSIGNED_BYTE;
            case VL_INT16:      return GL_SHORT;
            case VL_UINT16:     return GL_UNSIGNED_SHORT;
            case VL_INT32:      return GL_INT;
            case VL_UINT32:     return GL_UNSIGNED_INT;
            case VL_INT64:      return GL_INT64_ARB;
            case VL_UINT64:     return GL_UNSIGNED_INT64_ARB;
            case VL_FLOAT16:    return GL_HALF_FLOAT;
            case VL_FLOAT32:    return GL_FLOAT;
            case VL_FLOAT64:    return GL_DOUBLE;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid data type {} given to converter, return default: GL_FLOAT", type);
                return GL_FLOAT;
            }
        }
    }

}

