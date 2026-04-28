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

        const GLenum attachmentType = getAttachmentType(desc.format);
        m_Storage->attachToFramebuffer(m_FrameBufferID, attachmentType);

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLDepthStencilAttachment with ID {} for framebuffer {}", m_Storage->getID(), m_FrameBufferID);
    }

    GLDepthStencilAttachment::~GLDepthStencilAttachment(){
        SPDLOG_LOGGER_TRACE(m_Logger, "Destroyed GLDepthStencilAttachment with ID {} for framebuffer {}", m_Storage->getID(), m_FrameBufferID);
    }

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

    GLenum GLDepthStencilAttachment::getAttachmentType(const VL_TEXTURE_FORMAT format) const {
        switch (format) {
            case VL_TEXTURE_DEPTH_16:
            case VL_TEXTURE_DEPTH_24:
            case VL_TEXTURE_DEPTH_32:
                return GL_DEPTH_ATTACHMENT;
            case VL_TEXTURE_DEPTH_24_STENCIL_8:
            case VL_TEXTURE_DEPTH_32_STENCIL_8:
                return GL_DEPTH_STENCIL_ATTACHMENT;
            default:
                SPDLOG_LOGGER_ERROR(m_Logger, "Unsupported depth stencil format {} for attachment {}. Defaulting to GL_DEPTH24_STENCIL8", format, m_Storage->getID());
                return GL_DEPTH_STENCIL_ATTACHMENT;
        }
    }

    /////////////////////// GLDefaultDepthStencilAttachment ///////////////////////

    GLDefaultDepthStencilAttachment::GLDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const Device &device):
    DepthStencilAttachment(desc, device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)) {
        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLDefaultDepthStencilAttachment");
    }

    GLDefaultDepthStencilAttachment::~GLDefaultDepthStencilAttachment() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Destroyed GLDefaultDepthStencilAttachment");
    }

    void GLDefaultDepthStencilAttachment::bind() const {
        // The default framebuffer's depth stencil attachment is implicitly bound when the framebuffer is bound,
        // so no action is needed here
    }

    void GLDefaultDepthStencilAttachment::bindShaderResource(const U32 slot) const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to bind default depth stencil attachment as shader resource at slot {}, this is not supported", slot);
    }

    void GLDefaultDepthStencilAttachment::clear() const {
        glClearNamedFramebufferfi(0, GL_DEPTH_STENCIL, 0, m_ClearDepth, m_ClearStencil);
    }

    void GLDefaultDepthStencilAttachment::onResize(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
        // The default framebuffer's attachments are managed by the windowing system and automatically resize with the
        // window, so no action is needed here
    }

    UP<Image::IImage> GLDefaultDepthStencilAttachment::getData() const {
        SPDLOG_LOGGER_WARN(m_Logger, "Attempted to get data from default depth stencil attachment, this is not supported");
        return nullptr;
    }

    U64 GLDefaultDepthStencilAttachment::getIdentifier() const {
        return 0; // The default framebuffer's depth stencil attachment does not have a unique identifier
    }
}
