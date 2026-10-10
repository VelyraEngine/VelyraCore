#include "../../../Pch.hpp"

#include "GLESTranslations.hpp"

namespace Velyra::Core::GLES {

    namespace {

        struct TypeInfo {
            VL_TYPE dataType;
            GLenum pixelType;
            bool isInteger;
            GLenum internalFormats[4]; // indexed by channel count - 1
        };

        // 8 and 16 bit unsigned types are normalized (same as the OpenGL translations), the other integers are not
        constexpr TypeInfo TYPE_INFOS[] = {
            {VL_UINT8,   GL_UNSIGNED_BYTE,  false, {GL_R8,      GL_RG8,      GL_RGB8,      GL_RGBA8}},
            {VL_INT8,    GL_BYTE,           true,  {GL_R8I,     GL_RG8I,     GL_RGB8I,     GL_RGBA8I}},
            {VL_UINT16,  GL_UNSIGNED_SHORT, false, {GL_R16_EXT, GL_RG16_EXT, GL_RGB16_EXT, GL_RGBA16_EXT}},
            {VL_INT16,   GL_SHORT,          true,  {GL_R16I,    GL_RG16I,    GL_RGB16I,    GL_RGBA16I}},
            {VL_UINT32,  GL_UNSIGNED_INT,   true,  {GL_R32UI,   GL_RG32UI,   GL_RGB32UI,   GL_RGBA32UI}},
            {VL_INT32,   GL_INT,            true,  {GL_R32I,    GL_RG32I,    GL_RGB32I,    GL_RGBA32I}},
            {VL_FLOAT16, GL_HALF_FLOAT,     false, {GL_R16F,    GL_RG16F,    GL_RGB16F,    GL_RGBA16F}},
            {VL_FLOAT32, GL_FLOAT,          false, {GL_R32F,    GL_RG32F,    GL_RGB32F,    GL_RGBA32F}},
        };

        constexpr GLenum PIXEL_FORMATS[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
        constexpr GLenum INTEGER_PIXEL_FORMATS[4] = {GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER};

        GLESFormatDesc makeDepthStencilDesc(const GLenum internalFormat, const GLenum pixelFormat, const GLenum pixelType,
            const VL_TYPE dataType, const VL_CHANNEL_FORMAT channelFormat) {
            GLESFormatDesc desc;
            desc.internalFormat = internalFormat;
            desc.pixelFormat = pixelFormat;
            desc.pixelType = pixelType;
            desc.dataType = dataType;
            desc.channelFormat = channelFormat;
            desc.textureSupported = true;
            desc.isDepthStencil = true;
            return desc;
        }

        bool hasColorBufferFloat() {
            // EXT_color_buffer_float is part of the ES 3.2 core
            return GLAD_GL_ES_VERSION_3_2 != 0 || GLAD_GL_EXT_color_buffer_float != 0;
        }

        bool hasColorBufferHalfFloat() {
            return hasColorBufferFloat() || GLAD_GL_EXT_color_buffer_half_float != 0;
        }

    }

    GLESFormatDesc getGLESFormatDesc(const VL_TEXTURE_FORMAT format) {
        switch (format) {
            case VL_TEXTURE_DEPTH_16:
                return makeDepthStencilDesc(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, VL_UINT16, VL_CHANNEL_R);
            case VL_TEXTURE_DEPTH_24:
                return makeDepthStencilDesc(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, VL_UINT32, VL_CHANNEL_R);
            case VL_TEXTURE_DEPTH_32:
                // ES has no integer 32 bit depth format, only the floating point one
                return makeDepthStencilDesc(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, VL_FLOAT32, VL_CHANNEL_R);
            case VL_TEXTURE_DEPTH_24_STENCIL_8:
                return makeDepthStencilDesc(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, VL_UINT32, VL_CHANNEL_R);
            case VL_TEXTURE_DEPTH_32_STENCIL_8:
                return makeDepthStencilDesc(GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, VL_FLOAT32, VL_CHANNEL_RG);
            default:
                break;
        }

        GLESFormatDesc desc;
        const VL_CHANNEL_FORMAT channelFormat = getTextureChannelFormat(format);
        const VL_TYPE dataType = getTextureDataType(format);
        if (channelFormat < VL_CHANNEL_R || channelFormat > VL_CHANNEL_RGBA) {
            return desc;
        }
        const Size channelIndex = static_cast<Size>(channelFormat) - 1;

        const TypeInfo* info = nullptr;
        for (const TypeInfo& candidate: TYPE_INFOS) {
            if (candidate.dataType == dataType) {
                info = &candidate;
                break;
            }
        }
        if (info == nullptr) {
            return desc;
        }

        desc.internalFormat = info->internalFormats[channelIndex];
        desc.pixelFormat = info->isInteger ? INTEGER_PIXEL_FORMATS[channelIndex] : PIXEL_FORMATS[channelIndex];
        desc.pixelType = info->pixelType;
        desc.dataType = dataType;
        desc.channelFormat = channelFormat;

        // RGB is texture-only in ES 3.0 for everything except the 8 bit unsigned normalized format
        const bool isRgb = channelFormat == VL_CHANNEL_RGB;
        bool filterable = false;
        desc.textureSupported = true;
        switch (dataType) {
            case VL_UINT8:
                desc.colorRenderable = true;
                filterable = true;
                break;
            case VL_UINT16:
                desc.textureSupported = GLAD_GL_EXT_texture_norm16 != 0;
                desc.colorRenderable = desc.textureSupported && !isRgb;
                filterable = true;
                break;
            case VL_FLOAT16:
                desc.colorRenderable = !isRgb && hasColorBufferHalfFloat();
                filterable = true;
                break;
            case VL_FLOAT32:
                desc.colorRenderable = !isRgb && hasColorBufferFloat();
                break;
            default: // integer formats
                desc.colorRenderable = !isRgb;
                break;
        }
        desc.mipmapCapable = desc.textureSupported && desc.colorRenderable && filterable;
        return desc;
    }

}
