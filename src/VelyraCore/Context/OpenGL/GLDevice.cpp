#include "../../Pch.hpp"

#include "GLDevice.hpp"

namespace Velyra::Core {

    std::string GLDevice::getAPIVersion() const {
        return getGlConstantStr(GL_VERSION);
    }

    std::string GLDevice::getDeviceName() const {
        return getGlConstantStr(GL_RENDERER);
    }

    std::string GLDevice::getDeviceVendor() const {
        return getGlConstantStr(GL_VENDOR);
    }

    std::string GLDevice::getShadingLanguageVersion() const {
        return getGlConstantStr(GL_SHADING_LANGUAGE_VERSION);
    }

    U32 GLDevice::getMaxFramebufferWidth() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_WIDTH);
    }

    U32 GLDevice::getMaxFramebufferHeight() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_HEIGHT);
    }

    U32 GLDevice::getMaxFramebufferColorAttachments() const {
        return getGlConstantInt(GL_MAX_COLOR_ATTACHMENTS);
    }

    U32 GLDevice::getMaxViewportWidth() const {
        I32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return static_cast<U32>(retVal[0]);
    }

    U32 GLDevice::getMaxViewportHeight() const {
        I32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return static_cast<U32>(retVal[1]);
    }

    U32 GLDevice::getMaxTextureSlots() const {
        return getGlConstantInt(GL_MAX_TEXTURE_IMAGE_UNITS);
    }

    U32 GLDevice::getMaxTextureSize() const {
        return getGlConstantInt(GL_MAX_TEXTURE_SIZE);
    }

    U32 GLDevice::getMaxConstantBufferSize() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BLOCK_SIZE);
    }

    U32 GLDevice::getMaxConstantBufferSlots() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    }

    U32 GLDevice::getMaxShaderStorageBufferSize() const {
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BLOCK_SIZE);
    }

    U32 GLDevice::getMaxShaderStorageBufferSlots() const {
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    }

    U32 GLDevice::getMaxVertexAttributes() const {
        return getGlConstantInt(GL_MAX_VERTEX_ATTRIBS);
    }

    U64 GLDevice::getMaxVertexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_VERTICES);
    }

    U64 GLDevice::getMaxIndexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_INDICES);
    }

    U64 GLDevice::getMaxIndex() const {
        return getGlConstantInt(GL_MAX_ELEMENT_INDEX);
    }

    bool GLDevice::isDoubleBuffered() const {
        return getGlConstantInt(GL_DOUBLEBUFFER) == 1;
    }

    std::string GLDevice::getGlConstantStr(const GLenum constant) {
        const GLubyte* str = glGetString(constant);
        if (!str) return {}; // handle possible nullptr
        return reinterpret_cast<const char*>(str);
    }

    U32 GLDevice::getGlConstantInt(const GLenum constant) {
        I32 retVal = 0;
        glGetIntegerv(constant, &retVal);
        return static_cast<U32>(retVal);
    }

}