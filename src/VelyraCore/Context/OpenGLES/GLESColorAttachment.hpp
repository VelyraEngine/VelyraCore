#pragma once

#include <VelyraCore/Context/ColorAttachment.hpp>

namespace Velyra::Core::GLES {

    class GLESDefaultColorAttachment : public ColorAttachment {
    public:
        GLESDefaultColorAttachment(const ColorAttachmentDesc& desc, const Device& device);

        ~GLESDefaultColorAttachment() override;

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
