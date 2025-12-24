#include "../../../Pch.hpp"

#include "Translations.hpp"
#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLenum gelGLShaderType(const VL_SHADER_TYPE type) {
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

}

