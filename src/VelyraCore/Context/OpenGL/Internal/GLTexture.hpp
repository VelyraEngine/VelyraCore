#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct GLTextureDesc {
        GLenum target = GL_TEXTURE_2D;
        Size width = 0;
        Size height = 0;
        VL_TEXTURE_FORMAT format = VL_TEXTURE_FORMAT_MAX_VALUE;
        const void* data = nullptr;
        VL_BUFFER_USAGE usage = VL_BUFFER_USAGE_DEFAULT;
        bool generateMipmap = false;
    };

    class GLTexture {
    public:
        GLTexture(const GLTextureDesc& desc, const Device& device);

        ~GLTexture();

        void bind() const;

        void bindShaderResource(U32 slot) const;

        void setData(const void* data, U32 x, U32 y, U32 width, U32 height);

        void copyFrom(const GLTexture& other);

        UP<Image::IImage> getData() const;

        [[nodiscard]] GLuint getTextureID() const {
            return m_TextureID;
        }

        [[nodiscard]] GLuint getPixelUnpackBufferID() const {
            return m_PixelUnpackBufferID;
        }

        [[nodiscard]] Size getWidth() const {
            return m_Width;
        }

        [[nodiscard]] Size getHeight() const {
            return m_Height;
        }

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const {
            return m_Format;
        }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const {
            return m_Usage;
        }

        [[nodiscard]] GLenum getGLTarget() const {
            return m_GLTarget;
        }

    private:
        const Utils::LogPtr m_Logger;
        const Device& m_Device;

        U32 m_TextureID = 0;
        U32 m_PixelUnpackBufferID = 0;
        U32 m_PixelPackBufferID = 0;
        Size m_Width = 0;
        Size m_Height = 0;
        VL_TEXTURE_FORMAT m_Format = VL_TEXTURE_FORMAT_MAX_VALUE;
        VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;

        GLenum m_GLTarget = GL_TEXTURE_2D;
    };
}