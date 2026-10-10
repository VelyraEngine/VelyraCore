#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core::GLES {

    struct GLESFormatDesc {
        GLenum internalFormat = 0;      // 0 when the VL_TEXTURE_FORMAT has no OpenGL ES equivalent
        GLenum pixelFormat = 0;         // format argument of glTexSubImage2D / glReadPixels
        GLenum pixelType = 0;           // type argument of glTexSubImage2D / glReadPixels
        VL_TYPE dataType = VL_TYPE_NONE;
        VL_CHANNEL_FORMAT channelFormat = VL_CHANNEL_R;

        // The capability flags depend on the extensions of the current context, glad must be initialized
        bool textureSupported = false;
        bool colorRenderable = false;
        bool mipmapCapable = false;     // glGenerateMipmap requires a color-renderable and filterable format
        bool isDepthStencil = false;
    };

    /**
     * @brief Translates a VL_TEXTURE_FORMAT to the OpenGL ES 3.0 formats. Only extensions that are loaded by glad are
     *        taken into account (EXT_texture_norm16, EXT_color_buffer_float, EXT_color_buffer_half_float).
     */
    GLESFormatDesc getGLESFormatDesc(VL_TEXTURE_FORMAT format);

}
