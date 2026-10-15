#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    GLenum getGLShaderType(VL_SHADER_TYPE type);

    GLenum getGLBufferUsage(VL_BUFFER_USAGE usage);

    GLenum getGLDrawMode(VL_DRAW_MODE mode);

    GLenum getGLDataType(VL_TYPE type);

    GLenum getGLComparisonFunc(VL_COMPARISON_FUNC func);

    GLint getGLTextureFilter(VL_TEXTURE_FILTER filter);

    /**
     * @brief Combines the minification filter with the mip filter into a single GL min filter. A mip filter of
     *        VL_TEXTURE_FILTER_NONE disables mipmapping.
     */
    GLint getGLTextureMinFilter(VL_TEXTURE_FILTER minFilter, VL_TEXTURE_FILTER mipFilter);

    GLint getGLTextureWrap(VL_TEXTURE_WRAP wrap);

    GLenum getGLTextureFormat(VL_TEXTURE_FORMAT format);

    /**
     * @brief How the components of a texture format are exposed to GL. Integer formats need the *_INTEGER pixel
     *        formats for transfers and the iv/uiv variants for clears.
     */
    enum class GLComponentKind {
        Normalized, // unorm, float and depth/stencil formats
        SignedInteger,
        UnsignedInteger
    };

    GLComponentKind getGLComponentKind(VL_TEXTURE_FORMAT format);

    GLenum getGLTextureChannelFormat(VL_CHANNEL_FORMAT channelFormat, bool integer = false);

    GLenum getGLStencilFunc(VL_STENCIL_FUNC func);

    struct GLFormatDesc {
        VL_TYPE dataType;
        VL_CHANNEL_FORMAT channelFormat;
        GLenum glDataType;
        GLenum glChannelFormat;
    };

    GLFormatDesc getGLFormatDesc(VL_TEXTURE_FORMAT format);

}