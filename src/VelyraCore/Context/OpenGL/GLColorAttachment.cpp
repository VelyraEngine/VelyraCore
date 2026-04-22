#include "../../Pch.hpp"

#include "GLColorAttachment.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLTextureDesc createGLColorAttachmentDesc(const ColorAttachmentDesc& desc) {
        GLTextureDesc glDesc;
        glDesc.target = GL_TEXTURE_2D;
        glDesc.width = desc.width;
        glDesc.height = desc.height;
        glDesc.format = desc.format;
        glDesc.usage = desc.usage;
        return glDesc;
    }

    GLColorAttachment::GLColorAttachment(const ColorAttachmentDesc &desc, const Device &device, const U32 framebufferID, const U32 attachmentID):
    ColorAttachment(desc, device),
    m_Texture(createGLColorAttachmentDesc(desc), device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_FrameBufferID(framebufferID),
    m_AttachmentID(attachmentID){

        // TODO: Maybe this line can be moved to the framebuffer class?
        glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + m_AttachmentID, m_Texture.getTextureID(), 0);

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLColorAttachment with ID {} for framebuffer {} at attachment point {}", m_Texture.getTextureID(), m_FrameBufferID, m_AttachmentID);
    }

    GLColorAttachment::~GLColorAttachment() = default;

    void GLColorAttachment::bind() const {
        m_Texture.bind();
    }

    void GLColorAttachment::bindShaderResource(const U32 slot) const {
        m_Texture.bindShaderResource(slot);
    }

    void GLColorAttachment::clear() const {
        glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, static_cast<GLint>(m_AttachmentID), m_ClearColor.toArray());
    }

    void GLColorAttachment::onResize(const Size width, const Size height) {
        m_Texture.onResize(width, height);
    }

    UP<Image::IImage> GLColorAttachment::getData() const {
        return m_Texture.getData();
    }

    U64 GLColorAttachment::getIdentifier() const {
        return m_Texture.getTextureID();
    }
}
