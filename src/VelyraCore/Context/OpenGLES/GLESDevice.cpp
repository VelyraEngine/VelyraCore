#include "../../Pch.hpp"

#include "GLESDevice.hpp"
#include "Internal/GLESTranslations.hpp"

namespace Velyra::Core::GLES {

    std::string GLESDevice::getAPIVersion() const {
        return getGlConstantStr(GL_VERSION);
    }

    std::string GLESDevice::getDeviceName() const {
        return getGlConstantStr(GL_RENDERER);
    }

    std::string GLESDevice::getDeviceVendor() const {
        return getGlConstantStr(GL_VENDOR);
    }

    std::string GLESDevice::getShadingLanguageVersion() const {
        return getGlConstantStr(GL_SHADING_LANGUAGE_VERSION);
    }

    U32 GLESDevice::getMaxFramebufferWidth() const {
        // GL_MAX_FRAMEBUFFER_WIDTH only exists since ES 3.1 and describes attachment-less framebuffers
        return getGlConstantInt(GL_MAX_RENDERBUFFER_SIZE);
    }

    U32 GLESDevice::getMaxFramebufferHeight() const {
        return getGlConstantInt(GL_MAX_RENDERBUFFER_SIZE);
    }

    U32 GLESDevice::getMaxFramebufferColorAttachments() const {
        return getGlConstantInt(GL_MAX_COLOR_ATTACHMENTS);
    }

    U32 GLESDevice::getMaxViewportWidth() const {
        I32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return static_cast<U32>(retVal[0]);
    }

    U32 GLESDevice::getMaxViewportHeight() const {
        I32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return static_cast<U32>(retVal[1]);
    }

    U32 GLESDevice::getMaxTextureSlots() const {
        return getGlConstantInt(GL_MAX_TEXTURE_IMAGE_UNITS);
    }

    U32 GLESDevice::getMaxTextureSize() const {
        return getGlConstantInt(GL_MAX_TEXTURE_SIZE);
    }

    U32 GLESDevice::getMaxConstantBufferSize() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BLOCK_SIZE);
    }

    U32 GLESDevice::getMaxConstantBufferSlots() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    }

    U32 GLESDevice::getMaxVertexAttributes() const {
        return getGlConstantInt(GL_MAX_VERTEX_ATTRIBS);
    }

    Size GLESDevice::getMaxStructuredBufferSize() const {
        // Shader storage buffers are not part of ES 3.0
        if (!GLAD_GL_ES_VERSION_3_1) {
            return 0;
        }
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BLOCK_SIZE);
    }

    U32 GLESDevice::getMaxStructuredBufferSlots() const {
        if (!GLAD_GL_ES_VERSION_3_1) {
            return 0;
        }
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    }

    bool GLESDevice::isDoubleBuffered() const {
        // ES has no GL_DOUBLEBUFFER query, window surfaces created through GLFW/EGL are always back buffered
        return true;
    }

    bool GLESDevice::isTextureFormatSupported(const VL_TEXTURE_FORMAT format) const {
        return getGLESFormatDesc(format).textureSupported;
    }

    bool GLESDevice::isColorAttachmentFormatSupported(const VL_TEXTURE_FORMAT format) const {
        const GLESFormatDesc desc = getGLESFormatDesc(format);
        return desc.textureSupported && desc.colorRenderable;
    }

    bool GLESDevice::isDepthStencilAttachmentFormatSupported(const VL_TEXTURE_FORMAT format) const {
        const GLESFormatDesc desc = getGLESFormatDesc(format);
        return desc.textureSupported && desc.isDepthStencil;
    }

    std::string GLESDevice::getGlConstantStr(const GLenum constant) {
        const GLubyte* str = glGetString(constant);
        if (!str) return {};
        return reinterpret_cast<const char*>(str);
    }

    U32 GLESDevice::getGlConstantInt(const GLenum constant) {
        I32 retVal = 0;
        glGetIntegerv(constant, &retVal);
        return static_cast<U32>(retVal);
    }

}
