#include "../../../Pch.hpp"

#include "GLRenderBufferStorage.hpp"
#include "GLTranslations.hpp"

#include "../../../Logging/LoggerNames.hpp"

#include <VelyraImage/VelyraImage.hpp>

namespace Velyra::Core {

    GLRenderBufferStorage::GLRenderBufferStorage(const GLRenderBufferDesc& desc, const Device& device):
    m_RenderBuffer(desc, device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)) {
        // Create pixel pack buffer for efficient readback
        glCreateBuffers(1, &m_PixelPackBufferID);
    }

    GLRenderBufferStorage::~GLRenderBufferStorage() {
        if (m_PixelPackBufferID != 0) {
            glDeleteBuffers(1, &m_PixelPackBufferID);
        }
    }

    void GLRenderBufferStorage::attachToFramebuffer(const U32 framebufferID, const GLenum attachmentPoint) {
        m_FramebufferID = framebufferID;
        m_AttachmentPoint = attachmentPoint;
        glNamedFramebufferRenderbuffer(framebufferID, attachmentPoint, GL_RENDERBUFFER, m_RenderBuffer.getRenderBufferID());
    }

    void GLRenderBufferStorage::onResize(const Size width, const Size height) {
        m_RenderBuffer.onResize(width, height);
    }

    void GLRenderBufferStorage::bind() const {
        m_RenderBuffer.bind();
    }

    U64 GLRenderBufferStorage::getID() const {
        return m_RenderBuffer.getRenderBufferID();
    }

    Size GLRenderBufferStorage::getWidth() const {
        return m_RenderBuffer.getWidth();
    }

    Size GLRenderBufferStorage::getHeight() const {
        return m_RenderBuffer.getHeight();
    }

    VL_TEXTURE_FORMAT GLRenderBufferStorage::getFormat() const {
        return m_RenderBuffer.getFormat();
    }

    bool GLRenderBufferStorage::supportsShaderAccess() const {
        return false;
    }

    bool GLRenderBufferStorage::supportsReadback() const {
        return true;
    }

    void GLRenderBufferStorage::bindShaderResource(U32 /*slot*/) const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind RenderBuffer {} as shader resource - this operation is not supported for RenderBuffers", m_RenderBuffer.getRenderBufferID());
    }

    UP<Image::IImage> GLRenderBufferStorage::getData() const {
        VL_PRECONDITION(m_RenderBuffer.getRenderBufferID() != 0, "GLRenderBuffer object not created!");
        VL_PRECONDITION(m_FramebufferID != 0, "RenderBuffer not attached to a framebuffer!");

        // Extract format information
        const VL_TYPE dataType = getTextureDataType(m_RenderBuffer.getFormat());
        const VL_CHANNEL_FORMAT channelFormat = getTextureChannelFormat(m_RenderBuffer.getFormat());
        const U32 channelCount = Image::getChannelCountFromFormat(channelFormat);
        const Size dataSize = m_RenderBuffer.getWidth() * m_RenderBuffer.getHeight() * channelCount * Utils::getTypeSize(dataType);

        // Convert format information for OpenGL
        const GLenum glDataType = getGLDataType(dataType);
        const GLenum glChannelFormat = getGLTextureChannelFormat(channelFormat);

        // Bind the pixel pack buffer and allocate storage
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PixelPackBufferID);
        glBufferData(GL_PIXEL_PACK_BUFFER, static_cast<GLsizeiptr>(dataSize), nullptr, GL_STREAM_READ);

        // Bind framebuffer and set read buffer to our attachment
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferID);
        glReadBuffer(m_AttachmentPoint);

        // Read renderbuffer data into the pixel pack buffer via glReadPixels
        glReadPixels(0, 0, 
            static_cast<GLsizei>(m_RenderBuffer.getWidth()), 
            static_cast<GLsizei>(m_RenderBuffer.getHeight()),
            glChannelFormat, glDataType, nullptr); // nullptr indicates data goes to GL_PIXEL_PACK_BUFFER

        // Unbind framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        // Map the buffer to CPU memory for reading
        const void* mappedData = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        if (!mappedData) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to map pixel pack buffer for RenderBuffer {}", m_RenderBuffer.getRenderBufferID());
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            return nullptr;
        }

        // Create the appropriate image based on data type
        UP<Image::IImage> result = nullptr;
        
        if (dataType == VL_UINT8) {
            Image::ImageU8Desc imgDesc;
            imgDesc.width = m_RenderBuffer.getWidth();
            imgDesc.height = m_RenderBuffer.getHeight();
            imgDesc.format = channelFormat;
            imgDesc.data = static_cast<const U8*>(mappedData);
            result = Image::ImageFactory::createImageU8(imgDesc);
        } else if (dataType == VL_FLOAT32) {
            Image::ImageF32Desc imgDesc;
            imgDesc.width = m_RenderBuffer.getWidth();
            imgDesc.height = m_RenderBuffer.getHeight();
            imgDesc.format = channelFormat;
            imgDesc.data = static_cast<const float*>(mappedData);
            result = Image::ImageFactory::createImageF32(imgDesc);
        } else {
            SPDLOG_LOGGER_WARN(m_Logger, "Unsupported RenderBuffer data type {} for RenderBuffer {}. Only UI8 and F32 are currently supported for readback.", dataType, m_RenderBuffer.getRenderBufferID());
        }

        // Unmap and unbind
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        SPDLOG_LOGGER_TRACE(m_Logger, "Read {} bytes from RenderBuffer {} ({}x{}, format: {})", 
            dataSize, m_RenderBuffer.getRenderBufferID(), m_RenderBuffer.getWidth(), m_RenderBuffer.getHeight(), m_RenderBuffer.getFormat());

        return result;
    }

}
