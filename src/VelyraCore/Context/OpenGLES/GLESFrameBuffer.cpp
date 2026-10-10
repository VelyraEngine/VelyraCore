#include "../../Pch.hpp"

#include "GLESFrameBuffer.hpp"
#include "GLESColorAttachment.hpp"
#include "GLESDepthStencilAttachment.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core::GLES {

    GLESDefaultFrameBuffer::GLESDefaultFrameBuffer(const DefaultFrameBufferDesc& desc, const Device& device):
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)) {
        ColorAttachmentDesc caDesc;
        caDesc.enableShaderAccess = false;
        caDesc.clearColor = desc.clearColor;
        m_ColorAttachments.push_back(createUP<GLESDefaultColorAttachment>(caDesc, device));

        DepthStencilAttachmentDesc dsDesc;
        dsDesc.clearDepth = desc.clearDepth;
        dsDesc.clearStencil = desc.clearStencil;
        m_DepthStencilAttachment = createUP<GLESDefaultDepthStencilAttachment>(dsDesc, device);

        SPDLOG_LOGGER_DEBUG(m_Logger, "Default FrameBuffer created");
    }

    GLESDefaultFrameBuffer::~GLESDefaultFrameBuffer() {
        SPDLOG_LOGGER_DEBUG(m_Logger, "Default FrameBuffer destroyed");
    }

    void GLESDefaultFrameBuffer::begin() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Beginning Default FrameBuffer");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // ES has no glDrawBuffer, but glDrawBuffers accepts a single GL_BACK for the default framebuffer
        const GLenum backBuffer = GL_BACK;
        glDrawBuffers(1, &backBuffer);
    }

    void GLESDefaultFrameBuffer::end() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Ending Default FrameBuffer");
    }

    void GLESDefaultFrameBuffer::clear() {
        VL_PRECONDITION(!m_ColorAttachments.empty(), "Default FrameBuffer must have at least one color attachment");
        VL_PRECONDITION(m_DepthStencilAttachment != nullptr, "Default FrameBuffer must have a depth stencil attachment");

        m_ColorAttachments.front()->clear();
        m_DepthStencilAttachment->clear();
    }

    void GLESDefaultFrameBuffer::onResize(const Size /*width*/, const Size /*height*/) {

    }

}
