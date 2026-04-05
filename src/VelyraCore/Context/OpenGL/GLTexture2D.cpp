#include "../../Pch.hpp"

#include "GLTexture2D.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLTextureDesc createGLTexture2DDesc(const Texture2DDesc& desc) {
        GLTextureDesc glDesc;
        glDesc.target = GL_TEXTURE_2D;
        glDesc.width = desc.width;
        glDesc.height = desc.height;
        glDesc.format = desc.format;
        glDesc.data = desc.data;
        glDesc.usage = desc.usage;
        return glDesc;
    }

    GLTextureDesc createGLTexture2DDesc(const Texture2DImageDesc& desc) {
        GLTextureDesc glDesc;
        glDesc.target = GL_TEXTURE_2D;
        glDesc.width = desc.image->getWidth();
        glDesc.height = desc.image->getHeight();
        glDesc.format = getTextureFormat(desc.image);
        glDesc.data = desc.image->getData();
        glDesc.usage = desc.usage;
        return glDesc;
    }

    GLTexture2D::GLTexture2D(const Texture2DDesc &desc, const Device &device):
    Texture2D(desc, device),
    m_Texture(createGLTexture2DDesc(desc), device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)) {

    }

    GLTexture2D::GLTexture2D(const Texture2DImageDesc &desc, const Device &device):
    Texture2D(desc, device),
    m_Texture(createGLTexture2DDesc(desc), device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){

    }

    GLTexture2D::~GLTexture2D() = default;

    void GLTexture2D::bind() const {
        m_Texture.bind();
    }

    void GLTexture2D::bindShaderResource(const U32 slot) const {
        m_Texture.bindShaderResource(slot);
    }

    void GLTexture2D::setData(const void *data, const U32 x, const U32 y, const U32 width, const U32 height) {
        m_Texture.setData(data, x, y, width, height);
    }

    void GLTexture2D::copyFrom(const View<Texture2D>& other) {
        auto glTexture2D = dynamic_cast<const GLTexture2D*>(other.get());
        if (glTexture2D == nullptr) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to copy from texture2D object {} to texture2D object {}! Source "
                                          "texture is not of type GLTexture2D!", glTexture2D->getIdentifier(), getIdentifier());
        }
        m_Texture.copyFrom(glTexture2D->m_Texture);
    }

    UP<Image::IImage> GLTexture2D::getData() const {
        return m_Texture.getData();
    }

    U64 GLTexture2D::getIdentifier() const {
        return m_Texture.getTextureID();
    }
}
