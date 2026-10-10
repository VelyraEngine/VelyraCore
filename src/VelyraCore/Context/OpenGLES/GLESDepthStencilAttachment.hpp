#pragma once

#include <VelyraCore/Context/DepthStencilAttachment.hpp>

namespace Velyra::Core::GLES {

    class GLESDefaultDepthStencilAttachment: public DepthStencilAttachment {
    public:
        GLESDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Device& device);

        ~GLESDefaultDepthStencilAttachment() override;

        void bind() const override;

        void bindShaderResource(U32 slot) const override;

        void clear() const override;

        void onResize(Size width, Size height) override;

        [[nodiscard]] UP<Image::IImage> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        const Utils::LogPtr m_Logger;
    };
}
