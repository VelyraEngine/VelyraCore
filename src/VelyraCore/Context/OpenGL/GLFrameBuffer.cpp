#include "../../Pch.hpp"

#include "GLFrameBuffer.hpp"
#include "GLColorAttachment.hpp"
#include "GLDepthStencilAttachment.hpp"

#include "Internal/GLTextureStorage.hpp"
#include "Internal/GLRenderBufferStorage.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLFrameBuffer::GLFrameBuffer(const View<FrameBufferLayout> &layout, const Device& device):
    FrameBuffer(layout),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        glCreateFramebuffers(1, &m_FrameBufferID);
        createColorAttachments(layout, device);
        checkFrameBufferStatus();

        SPDLOG_LOGGER_TRACE(m_Logger, "FrameBuffer: {} created", m_FrameBufferID);
    }

    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &m_FrameBufferID);

        SPDLOG_LOGGER_TRACE(m_Logger, "FrameBuffer: {} destroyed", m_FrameBufferID);
    }

    void GLFrameBuffer::begin() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Beginning FrameBuffer: {}", m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
    }

    void GLFrameBuffer::end() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Ending FrameBuffer: {}", m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::clear() {
        for (const auto& attachment: m_ColorAttachments) {
            attachment->clear();
        }
        if (m_DepthStencilAttachment) {
            m_DepthStencilAttachment->clear();
        }
    }

    void GLFrameBuffer::onResize(const Size width, const Size height) {
        for (const auto& attachment: m_ColorAttachments) {
            attachment->onResize(width, height);
        }
        if (m_DepthStencilAttachment) {
            m_DepthStencilAttachment->onResize(width, height);
        }
    }

    void GLFrameBuffer::createColorAttachments(const View<FrameBufferLayout> &layout, const Device& device) {
        std::vector<GLenum> drawBuffers;
        U32 attachmentIndex = 0;
        for (const auto& attachDesc: layout->getColorAttachments()) {
            ColorAttachmentDesc desc;
            desc.width = layout->getWidth();
            desc.height = layout->getHeight();
            desc.clearColor = attachDesc.clearColor;
            desc.format = attachDesc.format;
            desc.usage = attachDesc.usage;

            UP<IGLFramebufferStorage> storage = nullptr;

            if (attachDesc.enableShaderAccess) {
                // Use texture storage for shader-accessible attachments
                GLTextureDesc texDesc;
                texDesc.target = GL_TEXTURE_2D;
                texDesc.width = desc.width;
                texDesc.height = desc.height;
                texDesc.format = desc.format;
                texDesc.usage = desc.usage;
                texDesc.generateMipmap = false; // Mipmaps are not needed for framebuffer attachments
                storage = createUP<GLTextureStorage>(texDesc, device);
            }
            else {
                // Use renderbuffer storage for non-shader-accessible attachments (more efficient)
                GLRenderBufferDesc rbDesc;
                rbDesc.width = desc.width;
                rbDesc.height = desc.height;
                rbDesc.format = desc.format;
                storage = createUP<GLRenderBufferStorage>(rbDesc, device);
            }

            auto attachment = createUP<GLColorAttachment>(desc, device, m_FrameBufferID, attachmentIndex, std::move(storage));
            m_ColorAttachments.push_back(std::move(attachment));

            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + attachmentIndex);
            attachmentIndex++;
        }
        glNamedFramebufferDrawBuffers(m_FrameBufferID, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
    }

    void GLFrameBuffer::createDepthStencilAttachment(const View<FrameBufferLayout> &layout, const Device &device) {
        const auto& optDesc = layout->getDepthStencilAttachment();
        if (!optDesc.has_value()) {
            return;
        }
        const FrameBufferDepthStencilAttachmentDesc& desc = optDesc.value();
        DepthStencilAttachmentDesc dsDesc;
        dsDesc.width = layout->getWidth();
        dsDesc.height = layout->getHeight();
        dsDesc.clearDepth = desc.clearDepth;
        dsDesc.clearStencil = desc.clearStencil;
        dsDesc.format = desc.format;
        UP<IGLFramebufferStorage> storage = nullptr;
        if (desc.enableShaderAccess) {
            // Use texture storage for shader-accessible attachments
            GLTextureDesc texDesc;
            texDesc.target = GL_TEXTURE_2D;
            texDesc.width = dsDesc.width;
            texDesc.height = dsDesc.height;
            texDesc.format = dsDesc.format;
            texDesc.usage = desc.usage;
            texDesc.generateMipmap = false; // Mipmaps are not needed for framebuffer attachments
            storage = createUP<GLTextureStorage>(texDesc, device);
        }
        else {
            // Use renderbuffer storage for non-shader-accessible attachments (more efficient)
            GLRenderBufferDesc rbDesc;
            rbDesc.width = dsDesc.width;
            rbDesc.height = dsDesc.height;
            rbDesc.format = dsDesc.format;
            storage = createUP<GLRenderBufferStorage>(rbDesc, device);
        }
        m_DepthStencilAttachment = createUP<GLDepthStencilAttachment>(dsDesc, device, m_FrameBufferID, std::move(storage));
    }

    GLenum GLFrameBuffer::checkFrameBufferStatus() {
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

