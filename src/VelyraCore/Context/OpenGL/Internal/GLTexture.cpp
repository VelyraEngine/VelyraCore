#include "../../../Pch.hpp"

#include "GLTexture.hpp"
#include "GLTranslations.hpp"

#include "../../../Logging/LoggerNames.hpp"

#include <VelyraImage/VelyraImage.hpp>

namespace Velyra::Core {

    namespace {
        // Full mip chain down to 1x1 when mipmaps are requested, otherwise only the base level
        GLsizei getMipLevelCount(const bool generateMipmap, const Size width, const Size height) {
            if (!generateMipmap) {
                return 1;
            }
            return static_cast<GLsizei>(std::bit_width(std::max<Size>({width, height, 1})));
        }
    }

    GLTexture::GLTexture(const GLTextureDesc& desc, const Device &device):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_Device(device),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_Usage(desc.usage),
    m_GenerateMipmap(desc.generateMipmap),
    m_GLTarget(desc.target) {
        if (!m_Device.isTextureFormatSupported(desc.format)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Texture format {} is not supported by the device", desc.format);
            return;
        }

        // Create the actual texture object and potentially fill it
        glCreateTextures(desc.target, 1, &m_TextureID);
        const GLenum internalFormat = getGLTextureFormat(desc.format);
        glTextureStorage2D(m_TextureID, getMipLevelCount(m_GenerateMipmap, m_Width, m_Height), internalFormat, static_cast<GLint>(m_Width), static_cast<GLint>(m_Height));
        if (desc.data != nullptr) {
            const GLFormatDesc transfer = getGLFormatDesc(desc.format);
            glTextureSubImage2D(m_TextureID, 0, 0, 0, static_cast<GLint>(m_Width), static_cast<GLint>(m_Height), transfer.glChannelFormat, transfer.glDataType, desc.data);
        }
        // Only generate mipmaps if requested
        if (desc.generateMipmap) {
            glGenerateTextureMipmap(m_TextureID);
        }
        // The PBO is used for texture updates, so we only create it if the usage is not static
        if (m_Usage != VL_BUFFER_USAGE_STATIC) {
            glCreateBuffers(1, &m_PixelUnpackBufferID);
            glCreateBuffers(1, &m_PixelPackBufferID);
        }

        SPDLOG_LOGGER_DEBUG(m_Logger, "Texture object {} created! (width: {}, height: {}, format: {}, usage: {})",
            m_TextureID, m_Width, m_Height, desc.format, desc.usage);
    }

    GLTexture::~GLTexture() {
        if (m_PixelUnpackBufferID != 0) {
            glDeleteBuffers(1, &m_PixelUnpackBufferID);
        }
        if (m_PixelPackBufferID != 0) {
            glDeleteBuffers(1, &m_PixelPackBufferID);
        }
        glDeleteTextures(1, &m_TextureID);

        SPDLOG_LOGGER_DEBUG(m_Logger, "Texture object {} deleted!", m_TextureID);
    }

    void GLTexture::bind() const {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        SPDLOG_LOGGER_TRACE(m_Logger, "Binding texture object {} to target {}", m_TextureID, m_GLTarget);
        glBindTexture(m_GLTarget, m_TextureID);
    }

    void GLTexture::bindShaderResource(const U32 slot) const {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        if (slot >= m_Device.getMaxTextureSlots()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Supplied slot {} is greater than the device number of texture slots {}", slot, m_Device.getMaxTextureSlots());
            return;
        }
        SPDLOG_LOGGER_TRACE(m_Logger, "Binding texture object {} to slot {}", m_TextureID, slot);
        glBindTextureUnit(slot, m_TextureID);
    }

    void GLTexture::onResize(const Size width, const Size height) {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        m_Width = width;
        m_Height = height;
        // glTextureStorage2D makes the storage immutable, so a resize needs a new texture object (contents are lost)
        glDeleteTextures(1, &m_TextureID);
        glCreateTextures(m_GLTarget, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, getMipLevelCount(m_GenerateMipmap, width, height), getGLTextureFormat(m_Format), static_cast<GLint>(width), static_cast<GLint>(height));

        SPDLOG_LOGGER_TRACE(m_Logger, "Resized texture object to new dimensions: {}x{} (new id: {})", width, height, m_TextureID);
    }

    void GLTexture::setData(const void *data, const U32 x, const U32 y, const U32 width, const U32 height) {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Update on texture object {} requested, which was created with {} usage flag!", m_TextureID, m_Usage);
            return;
        }
        VL_ASSERT(m_PixelUnpackBufferID != 0, "GLTexture", "Pixel unpack buffer is not created for texture object {}", m_TextureID);
        if (x + width > m_Width) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Width {} with offset {} exceeds the texture width {}. No data will be written!", width, x, m_Width);
            return;
        }
        if (y + height > m_Height) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Height {} with offset {} exceeds the texture height {}. No data will be written!", height, y, m_Height);
            return;
        }

        // Conversion variables
        const GLFormatDesc transfer = getGLFormatDesc(m_Format);
        const U32 channelCount = Image::getChannelCountFromFormat(transfer.channelFormat);

        // use the pbo for more efficient data transfer
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PixelUnpackBufferID);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * channelCount * Utils::getTypeSize(transfer.dataType), data, GL_DYNAMIC_DRAW);
        // With a PBO bound the data argument is an offset into that buffer, hence the nullptr
        glTextureSubImage2D(m_TextureID, 0,
            static_cast<GLint>(x), static_cast<GLint>(y),
            static_cast<GLint>(width), static_cast<GLint>(height),
            transfer.glChannelFormat, transfer.glDataType, nullptr
        );
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        if (m_GenerateMipmap) {
            glGenerateTextureMipmap(m_TextureID);
        }
    }

    void GLTexture::copyFrom(const GLTexture &other) {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");
        VL_PRECONDITION(other.m_TextureID != 0, "GLTexture Source texture object not created!");

        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Copy on texture object {} requested, which has been created with {} usage flag!", m_TextureID, m_Usage);
            return;
        }
        if (m_Width != other.m_Width || m_Height != other.m_Height) {
            SPDLOG_LOGGER_WARN(m_Logger, "Dimensions of the source texture ({}x{}) do not match the dimensions of the "
                                         "destination texture ({}x{}). The copy operation will be attempted but may fail!",
                                         other.m_Width, other.m_Height, m_Width, m_Height);
        }

        const auto width = static_cast<GLint>(std::min(m_Width, other.m_Width));
        const auto height = static_cast<GLint>(std::min(m_Height, other.m_Height));
        glCopyImageSubData(other.m_TextureID, other.m_GLTarget, 0, 0, 0, 0,
                           m_TextureID, m_GLTarget, 0, 0, 0, 0,
                           width, height, 1);

        if (m_GenerateMipmap) {
            glGenerateTextureMipmap(m_TextureID);
        }
    }

    UP<Image::IImage> GLTexture::getData() const {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        // Prepare pixel pack buffer for reading
        if (m_PixelPackBufferID == 0) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Pixel pack buffer not created for texture {}. Cannot read data.", m_TextureID);
            return nullptr;
        }

        // Extract format information
        const auto [dataType, channelFormat, glDataType, glChannelFormat] = getGLFormatDesc(m_Format);
        const U32 channelCount = Image::getChannelCountFromFormat(channelFormat);
        const Size dataSize = m_Width * m_Height * channelCount * Utils::getTypeSize(dataType);

        // Bind the pixel pack buffer and allocate storage
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PixelPackBufferID);
        glBufferData(GL_PIXEL_PACK_BUFFER, static_cast<GLsizeiptr>(dataSize), nullptr, GL_STREAM_READ);

        // Read texture data into the pixel pack buffer
        // Using DSA function glGetTextureImage which reads into the currently bound GL_PIXEL_PACK_BUFFER
        glGetTextureImage(m_TextureID, 0, glChannelFormat, glDataType, static_cast<GLsizei>(dataSize), nullptr);

        // Map the buffer to CPU memory for reading
        const void* mappedData = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        if (!mappedData) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to map pixel pack buffer for texture {}", m_TextureID);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            return nullptr;
        }

        // Create the appropriate image based on data type
        UP<Image::IImage> result = nullptr;
        
        if (dataType == VL_UINT8) {
            Image::ImageU8Desc imgDesc;
            imgDesc.width = m_Width;
            imgDesc.height = m_Height;
            imgDesc.format = channelFormat;
            imgDesc.data = static_cast<const U8*>(mappedData);
            result = Image::ImageFactory::createImageU8(imgDesc);
        } else if (dataType == VL_FLOAT32) {
            Image::ImageF32Desc imgDesc;
            imgDesc.width = m_Width;
            imgDesc.height = m_Height;
            imgDesc.format = channelFormat;
            imgDesc.data = static_cast<const float*>(mappedData);
            result = Image::ImageFactory::createImageF32(imgDesc);
        } else {
            SPDLOG_LOGGER_WARN(m_Logger, "Unsupported texture data type {} for texture {}. Only UI8 and F32 are currently supported for readback.", dataType, m_TextureID);
        }

        // Unmap and unbind
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        SPDLOG_LOGGER_TRACE(m_Logger, "Read {} bytes from texture {} ({}x{}, format: {})", 
            dataSize, m_TextureID, m_Width, m_Height, m_Format);

        return result;
    }

}
