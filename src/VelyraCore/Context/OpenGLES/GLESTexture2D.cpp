#include "../../Pch.hpp"

#include "GLESTexture2D.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core::GLES {

    namespace {

        GLESTextureDesc createGLESTexture2DDesc(const Texture2DDesc& desc) {
            GLESTextureDesc glDesc;
            glDesc.width = desc.width;
            glDesc.height = desc.height;
            glDesc.format = desc.format;
            glDesc.data = desc.data;
            glDesc.usage = desc.usage;
            glDesc.generateMipmap = desc.generateMipmap;
            return glDesc;
        }

        GLESTextureDesc createGLESTexture2DDesc(const Texture2DImageDesc& desc) {
            GLESTextureDesc glDesc;
            glDesc.width = desc.image->getWidth();
            glDesc.height = desc.image->getHeight();
            glDesc.format = getTextureFormat(desc.image);
            glDesc.data = desc.image->getData();
            glDesc.usage = desc.usage;
            glDesc.generateMipmap = desc.generateMipmap;
            return glDesc;
        }

    }

    GLESTexture2D::GLESTexture2D(const Texture2DDesc& desc, const Device& device):
    Texture2D(desc, device),
    m_Texture(createGLESTexture2DDesc(desc), device),
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)) {

    }

    GLESTexture2D::GLESTexture2D(const Texture2DImageDesc& desc, const Device& device):
    Texture2D(desc, device),
    m_Texture(createGLESTexture2DDesc(desc), device),
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)) {

    }

    GLESTexture2D::~GLESTexture2D() = default;

    void GLESTexture2D::bind() const {
        m_Texture.bind();
    }

    void GLESTexture2D::bindShaderResource(const U32 slot) const {
        m_Texture.bindShaderResource(slot);
    }

    void GLESTexture2D::setData(const void* data, const U32 x, const U32 y, const U32 width, const U32 height) {
        m_Texture.setData(data, x, y, width, height);
    }

    void GLESTexture2D::copyFrom(const View<Texture2D>& other) {
        const auto* glesTexture2D = dynamic_cast<const GLESTexture2D*>(other.get());
        if (glesTexture2D == nullptr) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to copy to texture2D object {}! Source texture is not of type GLESTexture2D!", getIdentifier());
            return;
        }
        m_Texture.copyFrom(glesTexture2D->m_Texture);
    }

    UP<Image::IImage> GLESTexture2D::getData() const {
        return m_Texture.getData();
    }

    U64 GLESTexture2D::getIdentifier() const {
        return m_Texture.getTextureID();
    }
}
