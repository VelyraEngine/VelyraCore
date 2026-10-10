#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraCore/Context/Device.hpp>

#include "GLESTranslations.hpp"

namespace Velyra::Core::GLES {

    struct GLESTextureDesc {
        Size width = 0;
        Size height = 0;
        VL_TEXTURE_FORMAT format = VL_TEXTURE_FORMAT_MAX_VALUE;
        const void* data = nullptr;
        VL_BUFFER_USAGE usage = VL_BUFFER_USAGE_DEFAULT;
        bool generateMipmap = false;
    };

    class GLESTexture {
    public:
        GLESTexture(const GLESTextureDesc& desc, const Device& device);

        ~GLESTexture();

        GLESTexture(const GLESTexture&) = delete;

        GLESTexture& operator=(const GLESTexture&) = delete;

        void bind() const;

        void bindShaderResource(U32 slot) const;

        void setData(const void* data, U32 x, U32 y, U32 width, U32 height);

        void copyFrom(const GLESTexture& other);

        [[nodiscard]] UP<Image::IImage> getData() const;

        [[nodiscard]] GLuint getTextureID() const {
            return m_TextureID;
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

    private:
        const Utils::LogPtr m_Logger;
        const Device& m_Device;

        U32 m_TextureID = 0;
        U32 m_MipLevels = 1;
        Size m_Width = 0;
        Size m_Height = 0;
        VL_TEXTURE_FORMAT m_Format = VL_TEXTURE_FORMAT_MAX_VALUE;
        VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        GLESFormatDesc m_FormatDesc;
    };
}
