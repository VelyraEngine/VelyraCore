#include "../../../Pch.hpp"

#include "GLRenderBuffer.hpp"
#include "GLTranslations.hpp"

#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLRenderBuffer::GLRenderBuffer(const GLRenderBufferDesc& desc, const Device& device):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_Device(device),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format) {
        if (!m_Device.isTextureFormatSupported(desc.format)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "RenderBuffer format {} is not supported by the device", desc.format);
            return;
        }

        glCreateRenderbuffers(1, &m_RenderBufferID);
        glNamedRenderbufferStorage(m_RenderBufferID, getGLTextureFormat(desc.format), static_cast<GLint>(m_Width), static_cast<GLint>(m_Height));

        SPDLOG_LOGGER_TRACE(m_Logger, "RenderBuffer object {} created! (width: {}, height: {}, format: {})",
            m_RenderBufferID, m_Width, m_Height, desc.format);
    }

    GLRenderBuffer::~GLRenderBuffer() {
        glDeleteRenderbuffers(1, &m_RenderBufferID);

        SPDLOG_LOGGER_TRACE(m_Logger, "RenderBuffer object {} deleted!", m_RenderBufferID);
    }

    void GLRenderBuffer::onResize(const Size width, const Size height) {
        VL_PRECONDITION(m_RenderBufferID != 0, "GLRenderBuffer object not created!");

        m_Width = width;
        m_Height = height;
        glNamedRenderbufferStorage(m_RenderBufferID, getGLTextureFormat(m_Format), static_cast<GLint>(width), static_cast<GLint>(height));

        SPDLOG_LOGGER_TRACE(m_Logger, "Resized RenderBuffer object {} to new dimensions: {}x{}", m_RenderBufferID, width, height);
    }

}
