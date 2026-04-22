#pragma once

#include <VelyraCore/Context/ColorAttachment.hpp>

#include "Internal/GLTexture.hpp"

namespace Velyra::Core {

    class GLColorAttachment : public ColorAttachment {
    public:
        GLColorAttachment(const ColorAttachmentDesc& desc, const Device& device, U32 framebufferID, U32 attachmentID);

        ~GLColorAttachment() override;

        void bind() const override;

        void bindShaderResource(U32 slot) const override;

        void clear() const override;

        void onResize(Size width, Size height) override;

        [[nodiscard]] UP<Image::IImage> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        GLTexture m_Texture;
        const Utils::LogPtr m_Logger;

        const U32 m_FrameBufferID;
        const U32 m_AttachmentID;
    };



}