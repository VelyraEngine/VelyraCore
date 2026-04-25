#include "../../Pch.hpp"

#include "GLDepthStencilAttachment.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLDepthStencilAttachment::GLDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const Device &device,
        const U32 framebufferID, UP<IGLFramebufferStorage> storage):
    DepthStencilAttachment(desc, device),
    m_Storage(std::move(storage)),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_FrameBufferID(framebufferID) {
        VL_PRECONDITION(m_Storage != nullptr, "[GLDepthStencilAttachment]: Storage cannot be null");

        m_Storage->attachToFramebuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT);

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLDepthStencilAttachment with ID {} for framebuffer {}", m_Storage->getID(), m_FrameBufferID);
    }

    GLDepthStencilAttachment::~GLDepthStencilAttachment() = default;

    void GLDepthStencilAttachment::bind() const {
        m_Storage->bind();
    }

    void GLDepthStencilAttachment::bindShaderResource(const U32 slot) const {
        if (!m_Storage->supportsShaderAccess()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind depth stencil attachment {} as shader resource, but storage does not support shader access", m_Storage->getID());
            return;
        }
        m_Storage->bindShaderResource(slot);
    }

    void GLDepthStencilAttachment::clear() const {
        glClearNamedFramebufferfi(m_FrameBufferID, GL_DEPTH_STENCIL, 0, m_ClearDepth, m_ClearStencil);
    }

    void GLDepthStencilAttachment::onResize(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
        m_Storage->onResize(width, height);
    }

    UP<Image::IImage> GLDepthStencilAttachment::getData() const {
        if (!m_Storage->supportsReadback()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Attempted to read data from depth stencil attachment {}, but storage does not support readback", m_Storage->getID());
            return nullptr;
        }
        return m_Storage->getData();
    }

    U64 GLDepthStencilAttachment::getIdentifier() const {
        return m_Storage->getID();
    }

}
