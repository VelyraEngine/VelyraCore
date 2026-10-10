#include "../../../Pch.hpp"

#include "GLESTexture.hpp"
#include "GLESStateGuards.hpp"

#include "../../../Logging/LoggerNames.hpp"

#include <VelyraImage/VelyraImage.hpp>

#include <bit>

namespace Velyra::Core::GLES {

    namespace {

        void uploadSubImage(const GLESFormatDesc& formatDesc, const GLint x, const GLint y, const GLsizei width,
            const GLsizei height, const void* data) {
            // Tightly packed rows, the default alignment of 4 skews e.g. RGB8 data with an odd width
            const PixelStoreGuard alignment(GL_UNPACK_ALIGNMENT, 1);
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, formatDesc.pixelFormat, formatDesc.pixelType, data);
        }

        // Compacts RGBA pixels in place to the first channelCount channels of every pixel
        template<typename T>
        void dropChannels(std::vector<T>& rgba, const Size pixelCount, const U32 channelCount) {
            if (channelCount == 4) {
                return;
            }
            for (Size i = 0; i < pixelCount; ++i) {
                for (U32 channel = 0; channel < channelCount; ++channel) {
                    rgba[i * channelCount + channel] = rgba[i * 4 + channel];
                }
            }
            rgba.resize(pixelCount * channelCount);
        }

    }

    GLESTexture::GLESTexture(const GLESTextureDesc& desc, const Device& device):
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)),
    m_Device(device),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_Usage(desc.usage),
    m_FormatDesc(getGLESFormatDesc(desc.format)) {
        if (!m_Device.isTextureFormatSupported(desc.format)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Texture format {} is not supported by the device", desc.format);
            return;
        }
        if (m_Width == 0 || m_Height == 0 || m_Width > m_Device.getMaxTextureSize() || m_Height > m_Device.getMaxTextureSize()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Texture dimensions {}x{} are invalid, the maximum texture size is {}",
                m_Width, m_Height, m_Device.getMaxTextureSize());
            return;
        }

        bool mipmaps = desc.generateMipmap;
        if (mipmaps && !m_FormatDesc.mipmapCapable) {
            SPDLOG_LOGGER_WARN(m_Logger, "Texture format {} cannot generate mipmaps on OpenGL ES, the texture will have a single level", desc.format);
            mipmaps = false;
        }
        m_MipLevels = mipmaps ? static_cast<U32>(std::bit_width(std::max(m_Width, m_Height))) : 1;

        glGenTextures(1, &m_TextureID);
        {
            const Texture2DBindingGuard binding(m_TextureID);
            glTexStorage2D(GL_TEXTURE_2D, static_cast<GLsizei>(m_MipLevels), m_FormatDesc.internalFormat,
                static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
            if (desc.data != nullptr) {
                uploadSubImage(m_FormatDesc, 0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height), desc.data);
                if (m_MipLevels > 1) {
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
            }
        }

        SPDLOG_LOGGER_DEBUG(m_Logger, "Texture object {} created! (width: {}, height: {}, format: {}, usage: {}, mip levels: {})",
            m_TextureID, m_Width, m_Height, desc.format, desc.usage, m_MipLevels);
    }

    GLESTexture::~GLESTexture() {
        if (m_TextureID != 0) {
            glDeleteTextures(1, &m_TextureID);
        }

        SPDLOG_LOGGER_DEBUG(m_Logger, "Texture object {} deleted!", m_TextureID);
    }

    void GLESTexture::bind() const {
        VL_PRECONDITION(m_TextureID != 0, "GLESTexture object not created!");

        SPDLOG_LOGGER_TRACE(m_Logger, "Binding texture object {}", m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void GLESTexture::bindShaderResource(const U32 slot) const {
        VL_PRECONDITION(m_TextureID != 0, "GLESTexture object not created!");

        if (slot >= m_Device.getMaxTextureSlots()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Supplied slot {} is greater than the device number of texture slots {}", slot, m_Device.getMaxTextureSlots());
            return;
        }
        SPDLOG_LOGGER_TRACE(m_Logger, "Binding texture object {} to slot {}", m_TextureID, slot);
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void GLESTexture::setData(const void* data, const U32 x, const U32 y, const U32 width, const U32 height) {
        VL_PRECONDITION(m_TextureID != 0, "GLESTexture object not created!");
        VL_PRECONDITION(data != nullptr, "Data pointer is null for texture object {}", m_TextureID);

        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Update on texture object {} requested, which was created with {} usage flag!", m_TextureID, m_Usage);
            return;
        }
        if (static_cast<Size>(x) + width > m_Width) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Width {} with offset {} exceeds the texture width {}. No data will be written!", width, x, m_Width);
            return;
        }
        if (static_cast<Size>(y) + height > m_Height) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Height {} with offset {} exceeds the texture height {}. No data will be written!", height, y, m_Height);
            return;
        }

        const Texture2DBindingGuard binding(m_TextureID);
        uploadSubImage(m_FormatDesc, static_cast<GLint>(x), static_cast<GLint>(y),
            static_cast<GLsizei>(width), static_cast<GLsizei>(height), data);
        if (m_MipLevels > 1) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void GLESTexture::copyFrom(const GLESTexture& other) {
        VL_PRECONDITION(m_TextureID != 0, "GLESTexture object not created!");
        VL_PRECONDITION(other.m_TextureID != 0, "GLESTexture Source texture object not created!");

        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Copy on texture object {} requested, which has been created with {} usage flag!", m_TextureID, m_Usage);
            return;
        }
        if (m_Width != other.m_Width || m_Height != other.m_Height) {
            SPDLOG_LOGGER_WARN(m_Logger, "Dimensions of the source texture ({}x{}) do not match the dimensions of the "
                                         "destination texture ({}x{}). The copy operation will be attempted but may fail!",
                                         other.m_Width, other.m_Height, m_Width, m_Height);
        }

        const auto width = static_cast<GLsizei>(std::min(m_Width, other.m_Width));
        const auto height = static_cast<GLsizei>(std::min(m_Height, other.m_Height));

        if (GLAD_GL_ES_VERSION_3_2) {
            glCopyImageSubData(other.m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0,
                               m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0,
                               width, height, 1);
        }
        else {
            // Before ES 3.2 the only way to copy texels on the GPU is reading them from a framebuffer
            if (!other.m_FormatDesc.colorRenderable || m_FormatDesc.internalFormat != other.m_FormatDesc.internalFormat) {
                SPDLOG_LOGGER_ERROR(m_Logger, "Failed to copy from texture object {} to texture object {}! Before OpenGL ES 3.2 "
                                              "both textures must have the same color-renderable format", other.m_TextureID, m_TextureID);
                return;
            }

            GLuint readFrameBuffer = 0;
            glGenFramebuffers(1, &readFrameBuffer);
            {
                const FramebufferBindingGuard readBinding(GL_READ_FRAMEBUFFER, readFrameBuffer);
                glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, other.m_TextureID, 0);
                const GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
                if (status == GL_FRAMEBUFFER_COMPLETE) {
                    const Texture2DBindingGuard binding(m_TextureID);
                    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
                }
                else {
                    SPDLOG_LOGGER_ERROR(m_Logger, "Failed to copy from texture object {}: read framebuffer is incomplete (status: 0x{:X})",
                        other.m_TextureID, status);
                }
            }
            glDeleteFramebuffers(1, &readFrameBuffer);
        }

        if (m_MipLevels > 1) {
            const Texture2DBindingGuard binding(m_TextureID);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    UP<Image::IImage> GLESTexture::getData() const {
        VL_PRECONDITION(m_TextureID != 0, "GLESTexture object not created!");

        // ES has no glGetTexImage, so the texture is read through a framebuffer which restricts the supported formats
        if (m_FormatDesc.isDepthStencil) {
            SPDLOG_LOGGER_WARN(m_Logger, "Cannot read back texture {}: OpenGL ES does not support reading depth/stencil data", m_TextureID);
            return nullptr;
        }
        if (m_FormatDesc.dataType != VL_UINT8 && m_FormatDesc.dataType != VL_FLOAT32) {
            SPDLOG_LOGGER_WARN(m_Logger, "Unsupported texture data type {} for texture {}. Only UI8 and F32 are currently supported for readback.",
                m_FormatDesc.dataType, m_TextureID);
            return nullptr;
        }
        if (!m_FormatDesc.colorRenderable) {
            SPDLOG_LOGGER_WARN(m_Logger, "Cannot read back texture {}: format {} is not color-renderable on this context", m_TextureID, m_Format);
            return nullptr;
        }

        const auto width = static_cast<GLsizei>(m_Width);
        const auto height = static_cast<GLsizei>(m_Height);
        const Size pixelCount = m_Width * m_Height;
        const U32 channelCount = Image::getChannelCountFromFormat(m_FormatDesc.channelFormat);

        UP<Image::IImage> result = nullptr;
        GLuint readFrameBuffer = 0;
        glGenFramebuffers(1, &readFrameBuffer);
        {
            const FramebufferBindingGuard readBinding(GL_READ_FRAMEBUFFER, readFrameBuffer);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
            const GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                SPDLOG_LOGGER_ERROR(m_Logger, "Cannot read back texture {}: read framebuffer is incomplete (status: 0x{:X})", m_TextureID, status);
            }
            else {
                // Color reads are only guaranteed for RGBA, so read all four channels and drop the unused ones
                const PixelStoreGuard alignment(GL_PACK_ALIGNMENT, 1);
                if (m_FormatDesc.dataType == VL_UINT8) {
                    std::vector<U8> pixels(pixelCount * 4);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
                    dropChannels(pixels, pixelCount, channelCount);

                    Image::ImageU8Desc imgDesc;
                    imgDesc.width = m_Width;
                    imgDesc.height = m_Height;
                    imgDesc.format = m_FormatDesc.channelFormat;
                    imgDesc.data = pixels.data();
                    result = Image::ImageFactory::createImageU8(imgDesc);
                }
                else {
                    std::vector<float> pixels(pixelCount * 4);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, pixels.data());
                    dropChannels(pixels, pixelCount, channelCount);

                    Image::ImageF32Desc imgDesc;
                    imgDesc.width = m_Width;
                    imgDesc.height = m_Height;
                    imgDesc.format = m_FormatDesc.channelFormat;
                    imgDesc.data = pixels.data();
                    result = Image::ImageFactory::createImageF32(imgDesc);
                }
            }
        }
        glDeleteFramebuffers(1, &readFrameBuffer);

        SPDLOG_LOGGER_TRACE(m_Logger, "Read texture {} ({}x{}, format: {})", m_TextureID, m_Width, m_Height, m_Format);

        return result;
    }

}
