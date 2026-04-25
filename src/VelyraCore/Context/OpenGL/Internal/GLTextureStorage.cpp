#include "../../../Pch.hpp"

#include "GLTextureStorage.hpp"

namespace Velyra::Core {

    GLTextureStorage::GLTextureStorage(const GLTextureDesc& desc, const Device& device):
    m_Texture(desc, device) {
    }

    void GLTextureStorage::attachToFramebuffer(const U32 framebufferID, const GLenum attachmentPoint) {
        glNamedFramebufferTexture(framebufferID, attachmentPoint, m_Texture.getTextureID(), 0);
    }

    void GLTextureStorage::onResize(const Size width, const Size height) {
        m_Texture.onResize(width, height);
    }

    void GLTextureStorage::bind() const {
        m_Texture.bind();
    }

    U64 GLTextureStorage::getID() const {
        return m_Texture.getTextureID();
    }

    Size GLTextureStorage::getWidth() const {
        return m_Texture.getWidth();
    }

    Size GLTextureStorage::getHeight() const {
        return m_Texture.getHeight();
    }

    VL_TEXTURE_FORMAT GLTextureStorage::getFormat() const {
        return m_Texture.getFormat();
    }

    bool GLTextureStorage::supportsShaderAccess() const {
        return true;
    }

    bool GLTextureStorage::supportsReadback() const {
        return true;
    }

    void GLTextureStorage::bindShaderResource(const U32 slot) const {
        m_Texture.bindShaderResource(slot);
    }

    UP<Image::IImage> GLTextureStorage::getData() const {
        return m_Texture.getData();
    }

}
