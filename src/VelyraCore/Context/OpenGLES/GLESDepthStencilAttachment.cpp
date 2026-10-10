#include "../../Pch.hpp"

#include "GLESDepthStencilAttachment.hpp"
#include "Internal/GLESStateGuards.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core::GLES {

    GLESDefaultDepthStencilAttachment::GLESDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Device& device):
    DepthStencilAttachment(desc, device),
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)) {
        SPDLOG_LOGGER_DEBUG(m_Logger, "Created GLESDefaultDepthStencilAttachment");
    }

    GLESDefaultDepthStencilAttachment::~GLESDefaultDepthStencilAttachment() {
        SPDLOG_LOGGER_DEBUG(m_Logger, "Destroyed GLESDefaultDepthStencilAttachment");
    }

    void GLESDefaultDepthStencilAttachment::bind() const {
        // The default framebuffer's depth stencil attachment is implicitly bound when the framebuffer is bound,
        // so no action is needed here
    }

    void GLESDefaultDepthStencilAttachment::bindShaderResource(const U32 slot) const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind default depth stencil attachment as shader resource at slot {}, this is not supported", slot);
    }

    void GLESDefaultDepthStencilAttachment::clear() const {
        // ES has no named framebuffer clear, so the default framebuffer has to be bound while clearing
        const FramebufferBindingGuard binding(GL_DRAW_FRAMEBUFFER, 0);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, m_ClearDepth, static_cast<GLint>(m_ClearStencil));
    }

    void GLESDefaultDepthStencilAttachment::onResize(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
        // The default framebuffer's attachments are managed by the windowing system and automatically resize with the
        // window, so no action is needed here
    }

    UP<Image::IImage> GLESDefaultDepthStencilAttachment::getData() const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to get data from default depth stencil attachment, this is not supported");
        return nullptr;
    }

    U64 GLESDefaultDepthStencilAttachment::getIdentifier() const {
        return 0; // The default framebuffer's depth stencil attachment does not have a unique identifier
    }
}
