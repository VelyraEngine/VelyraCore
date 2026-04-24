#pragma once

#include "IGLFramebufferStorage.hpp"
#include "GLTexture.hpp"

namespace Velyra::Core {

    class GLTextureStorage : public IGLFramebufferStorage {
    public:
        GLTextureStorage(const GLTextureDesc& desc, const Device& device);

        ~GLTextureStorage() override = default;

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

        [[nodiscard]] GLTexture& getTexture() { return m_Texture; }

        [[nodiscard]] const GLTexture& getTexture() const { return m_Texture; }

    private:
        GLTexture m_Texture;
    };

}
