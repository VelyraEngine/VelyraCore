#include "../../Pch.hpp"

#include "GLESColorAttachment.hpp"
#include "Internal/GLESStateGuards.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core::GLES {

    GLESDefaultColorAttachment::GLESDefaultColorAttachment(const ColorAttachmentDesc& desc, const Device& device):
    ColorAttachment(desc, device),
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)) {
        SPDLOG_LOGGER_DEBUG(m_Logger, "Created GLESDefaultColorAttachment");
    }

    GLESDefaultColorAttachment::~GLESDefaultColorAttachment() = default;

    void GLESDefaultColorAttachment::bind() const {
        // No need to bind the default framebuffer's color attachment, as it is always bound to the framebuffer
    }

    void GLESDefaultColorAttachment::bindShaderResource(const U32 slot) const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind default color attachment as shader resource at slot {}, this is not supported", slot);
    }

    void GLESDefaultColorAttachment::clear() const {
        // ES has no named framebuffer clear, so the default framebuffer has to be bound while clearing
        const FramebufferBindingGuard binding(GL_DRAW_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, m_ClearColor.toArray());
    }

    void GLESDefaultColorAttachment::onResize(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
    }

    UP<Image::IImage> GLESDefaultColorAttachment::getData() const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to get data from default color attachment which is not supported");
        return nullptr;
    }

    U64 GLESDefaultColorAttachment::getIdentifier() const {
        return 0; // The default framebuffer's color attachment does not have a texture ID, so we return 0
    }
}
