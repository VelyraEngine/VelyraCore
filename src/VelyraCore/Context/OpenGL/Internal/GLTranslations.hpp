#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    GLenum getGLShaderType(VL_SHADER_TYPE type);

    GLenum getGLBufferUsage(VL_BUFFER_USAGE usage);

    GLenum getGLDrawMode(VL_DRAW_MODE mode);

    GLenum getGLDataType(VL_TYPE type);

}