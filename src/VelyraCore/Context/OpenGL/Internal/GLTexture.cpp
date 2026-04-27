#include "../../../Pch.hpp"

#include "GLTexture.hpp"
#include "GLTranslations.hpp"

#include "../../../Logging/LoggerNames.hpp"

#include <VelyraImage/VelyraImage.hpp>

namespace Velyra::Core {

    GLTexture::GLTexture(const GLTextureDesc& desc, const Device &device):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_Device(device),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_Usage(desc.usage),
    m_GLTarget(desc.target) {
        if (!m_Device.isTextureFormatSupported(desc.format)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Texture format {} is not supported by the device", desc.format);
            return;
        }

        // Create the actual texture object and potentially fill it
        glCreateTextures(desc.target, 1, &m_TextureID);
        const GLenum internalFormat = getGLTextureFormat(desc.format);
        glTextureStorage2D(m_TextureID, 1, internalFormat, static_cast<GLint>(m_Width), static_cast<GLint>(m_Height));
        if (desc.data != nullptr) {
            const VL_TYPE vlType = getTextureDataType(desc.format);
            const VL_CHANNEL_FORMAT vlChannelFormat = getTextureChannelFormat(desc.format);
            const GLenum glDataType = getGLDataType(vlType);
            const GLenum glChannelFormat = getGLTextureChannelFormat(vlChannelFormat);
            glTextureSubImage2D(m_TextureID, 0, 0, 0, static_cast<GLint>(m_Width), static_cast<GLint>(m_Height), glChannelFormat, glDataType, desc.data);
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

        SPDLOG_LOGGER_TRACE(m_Logger, "Texture object {} created! (width: {}, height: {}, format: {}, usage: {})",
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

        SPDLOG_LOGGER_TRACE(m_Logger, "Texture object {} deleted!", m_TextureID);
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
        glTextureStorage2D(m_TextureID, 1, getGLTextureFormat(m_Format), static_cast<GLint>(width), static_cast<GLint>(height));

        SPDLOG_LOGGER_TRACE(m_Logger, "Resized texture object {} to new dimensions: {}x{}", m_TextureID, width, height);
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
        const VL_CHANNEL_FORMAT vlChannelFormat = getTextureChannelFormat(m_Format);
        const VL_TYPE vlType = getTextureDataType(m_Format);
        const GLenum glDataType = getGLDataType(vlType);
        const GLenum glChannelFormat = getGLTextureChannelFormat(vlChannelFormat);
        const U32 channelCount = Image::getChannelCountFromFormat(vlChannelFormat);

        // use the pbo for more efficient data transfer
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PixelUnpackBufferID);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * channelCount * Utils::getTypeSize(vlType), data, GL_DYNAMIC_DRAW);
        /*
         * DSA does currently not provide a function like glNamedReadPixelsToBuffer. so we have to bind the texture to the context
         */
        glBindTexture(m_GLTarget, m_TextureID);
        glTexSubImage2D(m_GLTarget, 0,
            static_cast<GLint>(x), static_cast<GLint>(y),
            static_cast<GLint>(width), static_cast<GLint>(height),
            glChannelFormat, glDataType, nullptr // nullptr is used to indicate that the data is stored in the buffer bound to GL_PIXEL_UNPACK_BUFFER
        );

        // Cleanup bindings
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        glBindTexture(m_GLTarget, 0);
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

    }

    UP<Image::IImage> GLTexture::getData() const {
        VL_PRECONDITION(m_TextureID != 0, "GLTexture object not created!");

        // Prepare pixel pack buffer for reading
        if (m_PixelPackBufferID == 0) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Pixel pack buffer not created for texture {}. Cannot read data.", m_TextureID);
            return nullptr;
        }

        // Extract format information
        const auto [dataType, channelFormat, glDataType, glChannelFormat] = getFormatDesc();
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

    GLTexture::FormatDesc GLTexture::getFormatDesc() const {
        // Filter out depth stencil formats, we will determine the data layout separately
        FormatDesc desc{};
        switch (m_Format) {
            case VL_TEXTURE_DEPTH_16:
                /*
                 * It seems that OpenGL only supports 32-bit depth formats. So even when DEPTH_16 was requested
                 * F32 will still be used as a storage
                 */
                desc.dataType = VL_FLOAT32;
                desc.channelFormat = VL_CHANNEL_R;
                desc.glDataType = GL_FLOAT;
                desc.glChannelFormat = GL_DEPTH_COMPONENT;
                break;
            case VL_TEXTURE_DEPTH_24:
                desc.dataType = VL_FLOAT32; // Depth24 is typically stored in a 32-bit format for alignment reasons, even though only 24 bits are used for depth
                desc.channelFormat = VL_CHANNEL_R;
                desc.glDataType = GL_FLOAT;
                desc.glChannelFormat = GL_DEPTH_COMPONENT;
                break;
            case VL_TEXTURE_DEPTH_32:
                desc.dataType = VL_FLOAT32;
                desc.channelFormat = VL_CHANNEL_R;
                desc.glDataType = GL_FLOAT;
                desc.glChannelFormat = GL_DEPTH_COMPONENT;
                break;
            case VL_TEXTURE_DEPTH_24_STENCIL_8:
                /*
                 * Packed format where 24 bits are used for depth and 8 bits for stencil. We have 2 options
                 *   1. dataType = VL_UINT32 && channelFormat = VL_CHANNEL_R
                 *   2. dataType = VL_UINT8 && channelFormat = VL_CHANNEL_RGBA
                 *
                 *   -> We let the user decide how to interpret the data, so VL_UINT32 it is
                 */
                desc.dataType = VL_UINT32;
                desc.channelFormat = VL_CHANNEL_R;
                desc.glDataType = GL_UNSIGNED_INT_24_8; // This is a special OpenGL data type that indicates the packed depth-stencil format
                desc.glChannelFormat = GL_DEPTH_STENCIL; // This indicates that the data contains both depth and stencil information
                break;
            case VL_TEXTURE_DEPTH_32_STENCIL_8:
                VL_THROW("Unsupported readback format! OpenGL does NOT support reading back from DEPTH_32_STENCIL_8 "
                         "textures. This is a hardware limitation that cannot be worked around. If you need to read back"
                         " depth and stencil data, consider using DEPTH_24_STENCIL_8 format instead, which can be read"
                         " back as a single 32-bit unsigned integer where the depth and stencil values are packed together.");
            default:
                desc.dataType = getTextureDataType(m_Format);
                desc.channelFormat = getTextureChannelFormat(m_Format);
                desc.glDataType = getGLDataType(desc.dataType);
                desc.glChannelFormat = getGLTextureChannelFormat(desc.channelFormat);
                break;

        }
        return desc;
    }
}
