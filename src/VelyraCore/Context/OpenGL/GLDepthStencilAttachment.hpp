#pragma once

#include <VelyraCore/Context/DepthStencilAttachment.hpp>

#include "Internal/IGLFramebufferStorage.hpp"

namespace Velyra::Core {

    class GLDepthStencilAttachment: public DepthStencilAttachment {
    public:
        GLDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Device& device, U32 framebufferID, UP<IGLFramebufferStorage> storage);

        ~GLDepthStencilAttachment() override;

        void bind() const override;

        void bindShaderResource(U32 slot) const override;

        void clear() const override;

        void onResize(Size width, Size height) override;

        [[nodiscard]] UP<Image::IImage> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:

        GLenum getAttachmentType(VL_TEXTURE_FORMAT format) const;

    private:
        UP<IGLFramebufferStorage> m_Storage;
        const Utils::LogPtr m_Logger;

        const U32 m_FrameBufferID;
    };
}