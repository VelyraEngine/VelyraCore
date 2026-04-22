#include "../../Pch.hpp"

#include "GLRenderPass.hpp"
#include "GLColorAttachment.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLRenderPass::GLRenderPass(const View<RenderPassLayout> &layout, const Device& device):
    RenderPass(layout),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        glCreateFramebuffers(1, &m_FrameBufferID);
        createColorAttachments(layout, device);
        checkFrameBufferStatus();

        SPDLOG_LOGGER_TRACE(m_Logger, "RenderPass: {} created", m_FrameBufferID);
    }

    GLRenderPass::~GLRenderPass() {
        glDeleteFramebuffers(1, &m_FrameBufferID);

        SPDLOG_LOGGER_TRACE(m_Logger, "RenderPass: {} destroyed", m_FrameBufferID);
    }

    void GLRenderPass::begin() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Beginning RenderPass: {}", m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
    }

    void GLRenderPass::end() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Ending RenderPass: {}", m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLRenderPass::clear() {
        for (const auto& attachment: m_ColorAttachments) {
            attachment->clear();
        }
    }

    void GLRenderPass::clear(const Size attachmentIndex) {
        if (attachmentIndex >= m_ColorAttachments.size()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to clear color attachment at index {} which is out of bounds for render pass {}", attachmentIndex, m_FrameBufferID);
            return;
        }
        m_ColorAttachments.at(attachmentIndex)->clear();

    }

    void GLRenderPass::onResize(const Size width, const Size height) {
        for (const auto& attachment: m_ColorAttachments) {
            attachment->onResize(width, height);
        }
    }

    void GLRenderPass::onResize(const Size width, const Size height, const Size attachmentIndex) {
        if (attachmentIndex >= m_ColorAttachments.size()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to resize color attachment at index {} which is out of bounds for render pass {}", attachmentIndex, m_FrameBufferID);
            return;
        }
        m_ColorAttachments.at(attachmentIndex)->onResize(width, height);

    }

    void GLRenderPass::createColorAttachments(const View<RenderPassLayout> &layout, const Device& device) {
        std::vector<GLenum> drawBuffers;
        U32 attachmentIndex = 0;
        for (const auto& attachDesc: layout->getColorAttachments()) {
            ColorAttachmentDesc desc;
            desc.width = layout->getWidth();
            desc.height = layout->getHeight();
            desc.clearColor = attachDesc.clearColor;
            desc.format = attachDesc.format;
            if (attachDesc.enableShaderAccess) {
                auto attachment = createUP<GLColorAttachment>(desc, device, m_FrameBufferID, attachmentIndex);
                m_ColorAttachments.push_back(std::move(attachment));
            }
            else {
                // TODO: Implement color attachments as render buffers as well!
            }

            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + attachmentIndex);
            attachmentIndex++;
        }
        glNamedFramebufferDrawBuffers(m_FrameBufferID, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
    }

    GLenum GLRenderPass::checkFrameBufferStatus() {
        const GLenum status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                break;
            case GL_FRAMEBUFFER_UNDEFINED:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} is undefined!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has incomplete attachment!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} is missing attachment!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has incomplete draw buffer!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has incomplete read buffer!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} is unsupported!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has incomplete multisample!", m_FrameBufferID);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has incomplete layer targets!", m_FrameBufferID);
                break;
            default:
                SPDLOG_LOGGER_ERROR(m_Logger, "Framebuffer {} has unknown error: {}", m_FrameBufferID, status);
                break;
        }
        return status;
    }
}

