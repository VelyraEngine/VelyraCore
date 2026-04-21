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

    GLint getGLComparisonFunc(const VL_COMPARISON_FUNC func) {
        switch (func) {
            case VL_COMPARISON_FUNC_NEVER:       return GL_NEVER;
            case VL_COMPARISON_FUNC_LESS:        return GL_LESS;
            case VL_COMPARISON_FUNC_EQUAL:       return GL_EQUAL;
            case VL_COMPARISON_FUNC_LESS_EQUAL:      return GL_LEQUAL;
            case VL_COMPARISON_FUNC_GREATER:     return GL_GREATER;
            case VL_COMPARISON_FUNC_NOT_EQUAL:   return GL_NOTEQUAL;
            case VL_COMPARISON_FUNC_GREATER_EQUAL:      return GL_GEQUAL;
            case VL_COMPARISON_FUNC_ALWAYS:      return GL_ALWAYS;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid comparison function {} given to converter, return default: GL_ALWAYS", func);
                return GL_ALWAYS;
            }
        }
    }

    GLint getGLTextureFilter(const VL_TEXTURE_FILTER filter) {
        switch (filter) {
            case VL_TEXTURE_FILTER_NONE:     return GL_NONE;
            case VL_TEXTURE_FILTER_LINEAR:   return GL_LINEAR;
            case VL_TEXTURE_FILTER_POINT:    return GL_NEAREST;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid texture filter {} given to converter, return default: GL_LINEAR", filter);
                return GL_LINEAR;
            }
        }
    }

    GLint getGLTextureWrap(const VL_TEXTURE_WRAP wrap) {
        switch (wrap) {
            case VL_TEXTURE_WRAP_NONE:           return GL_NONE;
            case VL_TEXTURE_WRAP_REPEAT:         return GL_REPEAT;
            case VL_TEXTURE_WRAP_MIRROR_REPEAT:  return GL_MIRRORED_REPEAT;
            case VL_TEXTURE_WRAP_CLAMP_EDGE:     return GL_CLAMP_TO_EDGE;
            case VL_TEXTURE_WRAP_CLAMP_BORDER:   return GL_CLAMP_TO_BORDER;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid texture wrap {} given to converter, return default: GL_REPEAT", wrap);
                return GL_REPEAT;
            }
        }
    }

    GLenum getGLTextureFormat(const VL_TEXTURE_FORMAT format) {
        switch (format) {
            case VL_TEXTURE_R_U8:      return GL_R8;
            case VL_TEXTURE_R_I8:      return GL_R8I;
            case VL_TEXTURE_R_U16:     return GL_R16;
            case VL_TEXTURE_R_I16:     return GL_R16I;
            case VL_TEXTURE_R_U32:     return GL_R32UI;
            case VL_TEXTURE_R_I32:     return GL_R32I;
            case VL_TEXTURE_R_F16:     return GL_R16F;
            case VL_TEXTURE_R_F32:     return GL_R32F;

            case VL_TEXTURE_RG_U8:     return GL_RG8;
            case VL_TEXTURE_RG_I8:     return GL_RG8I;
            case VL_TEXTURE_RG_U16:    return GL_RG16;
            case VL_TEXTURE_RG_I16:    return GL_RG16I;
            case VL_TEXTURE_RG_U32:    return GL_RG32UI;
            case VL_TEXTURE_RG_I32:    return GL_RG32I;
            case VL_TEXTURE_RG_F16:    return GL_RG16F;
            case VL_TEXTURE_RG_F32:    return GL_RG32F;

            case VL_TEXTURE_RGB_U8:    return GL_RGB8;
            case VL_TEXTURE_RGB_I8:    return GL_RGB8I;
            case VL_TEXTURE_RGB_U16:   return GL_RGB16;
            case VL_TEXTURE_RGB_I16:   return GL_RGB16I;
            case VL_TEXTURE_RGB_U32:   return GL_RGB32UI;
            case VL_TEXTURE_RGB_I32:   return GL_RGB32I;
            case VL_TEXTURE_RGB_F16:   return GL_RGB16F;
            case VL_TEXTURE_RGB_F32:   return GL_RGB32F;

            case VL_TEXTURE_RGBA_U8:   return GL_RGBA8;
            case VL_TEXTURE_RGBA_I8:   return GL_RGBA8I;
            case VL_TEXTURE_RGBA_U16:  return GL_RGBA16;
            case VL_TEXTURE_RGBA_I16:  return GL_RGBA16I;
            case VL_TEXTURE_RGBA_U32:  return GL_RGBA32UI;
            case VL_TEXTURE_RGBA_I32:  return GL_RGBA32I;
            case VL_TEXTURE_RGBA_F16:  return GL_RGBA16F;
            case VL_TEXTURE_RGBA_F32:  return GL_RGBA32F;

                // special formats
            case VL_TEXTURE_DEPTH_16:          return GL_DEPTH_COMPONENT16;
            case VL_TEXTURE_DEPTH_24:          return GL_DEPTH_COMPONENT24;
            case VL_TEXTURE_DEPTH_32:          return GL_DEPTH_COMPONENT32;
            case VL_TEXTURE_DEPTH_24_STENCIL_8:    return GL_DEPTH24_STENCIL8;
            case VL_TEXTURE_DEPTH_32_STENCIL_8:    return GL_DEPTH32F_STENCIL8;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid texture format {} given to converter, return default: GL_RGBA8", format);
                return GL_RGBA8;
            }
        }
    }

    GLenum getGLTextureChannelFormat(const VL_CHANNEL_FORMAT channelFormat) {
        switch (channelFormat) {
            case VL_CHANNEL_R: return GL_RED;
            case VL_CHANNEL_RG: return GL_RG;
            case VL_CHANNEL_RGB: return GL_RGB;
            case VL_CHANNEL_RGBA: return GL_RGBA;
            default: {
                const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
                SPDLOG_LOGGER_WARN(logger, "Invalid texture channel format {} given to converter, return default: GL_RGBA", channelFormat);
                return GL_RGBA;
            }
        }
    }
}

