#pragma once

#include "IGLFramebufferStorage.hpp"
#include "GLRenderBuffer.hpp"

namespace Velyra::Core {

    class GLRenderBufferStorage : public IGLFramebufferStorage {
    public:
        GLRenderBufferStorage(const GLRenderBufferDesc& desc, const Device& device);

        ~GLRenderBufferStorage() override;

        void attachToFramebuffer(U32 framebufferID, GLenum attachmentPoint) override;

        void onResize(Size width, Size height) override;

        [[nodiscard]] U64 getID() const override;

        [[nodiscard]] Size getWidth() const override;

        [[nodiscard]] Size getHeight() const override;

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const override;

        [[nodiscard]] bool supportsShaderAccess() const override;

        [[nodiscard]] bool supportsReadback() const override;

        void bindShaderResource(U32 slot) const override;

        [[nodiscard]] UP<Image::IImage> getData() const override;

        [[nodiscard]] GLRenderBuffer& getRenderBuffer() { return m_RenderBuffer; }

        [[nodiscard]] const GLRenderBuffer& getRenderBuffer() const { return m_RenderBuffer; }

    private:
        GLRenderBuffer m_RenderBuffer;
        const Utils::LogPtr m_Logger;
        
        U32 m_FramebufferID = 0;
        GLenum m_AttachmentPoint = GL_COLOR_ATTACHMENT0;
        U32 m_PixelPackBufferID = 0;
    };

}
