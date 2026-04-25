#include "../../Pch.hpp"

#include "GLColorAttachment.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLColorAttachment::GLColorAttachment(const ColorAttachmentDesc& desc, const Device& device, const U32 framebufferID, const U32 attachmentID, UP<IGLFramebufferStorage> storage):
    ColorAttachment(desc, device),
    m_Storage(std::move(storage)),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_FrameBufferID(framebufferID),
    m_AttachmentID(attachmentID) {
        VL_PRECONDITION(m_Storage != nullptr, "[GLColorAttachment]: Storage cannot be null");

        m_Storage->attachToFramebuffer(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + m_AttachmentID);

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLColorAttachment with ID {} for framebuffer {} at attachment point {}", m_Storage->getID(), m_FrameBufferID, m_AttachmentID);
    }

    GLColorAttachment::~GLColorAttachment() = default;

    void GLColorAttachment::bind() const {
        m_Storage->bind();
    }

    void GLColorAttachment::bindShaderResource(const U32 slot) const {
        if (!m_Storage->supportsShaderAccess()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind color attachment {} as shader resource, but storage does not support shader access", m_Storage->getID());
            return;
        }
        m_Storage->bindShaderResource(slot);
    }

    void GLColorAttachment::clear() const {
        glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, static_cast<GLint>(m_AttachmentID), m_ClearColor.toArray());
    }

    void GLColorAttachment::onResize(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
        m_Storage->onResize(width, height);
    }

    UP<Image::IImage> GLColorAttachment::getData() const {
        if (!m_Storage->supportsReadback()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to read data from color attachment {}, but storage does not support readback", m_Storage->getID());
            return nullptr;
        }
        return m_Storage->getData();
    }

    U64 GLColorAttachment::getIdentifier() const {
        return m_Storage->getID();
    }
}
