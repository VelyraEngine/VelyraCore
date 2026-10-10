#pragma once

#include <VelyraCore/Context/Texture2D.hpp>

#include "Internal/GLESTexture.hpp"

namespace Velyra::Core::GLES {

    class GLESTexture2D: public Texture2D {
    public:
        GLESTexture2D(const Texture2DDesc& desc, const Device& device);

        GLESTexture2D(const Texture2DImageDesc& desc, const Device& device);

        ~GLESTexture2D() override;

        void bind() const override;

        void bindShaderResource(U32 slot) const override;

        void setData(const void* data, U32 x, U32 y, U32 width, U32 height) override;

        void copyFrom(const View<Texture2D>& other) override;

        UP<Image::IImage> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        GLESTexture m_Texture;

        const Utils::LogPtr m_Logger;
    };
}
