#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    GLenum getGLShaderType(VL_SHADER_TYPE type);

    GLenum getGLBufferUsage(VL_BUFFER_USAGE usage);

    GLenum getGLDrawMode(VL_DRAW_MODE mode);

    GLenum getGLDataType(VL_TYPE type);

    GLenum getGLComparisonFunc(VL_COMPARISON_FUNC func);

    GLint getGLTextureFilter(VL_TEXTURE_FILTER filter);

    GLint getGLTextureWrap(VL_TEXTURE_WRAP wrap);

    GLenum getGLTextureFormat(VL_TEXTURE_FORMAT format);

    GLenum getGLTextureChannelFormat(VL_CHANNEL_FORMAT channelFormat);

    GLenum getGLStencilFunc(VL_STENCIL_FUNC func);

}