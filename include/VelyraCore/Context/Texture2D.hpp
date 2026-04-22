#pragma once

#include <VelyraCore/Context/Device.hpp>
#include <VelyraImage/IImage.hpp>

#include "VelyraUtils/Memory/View.hpp"

namespace Velyra::Core {

    struct VL_API Texture2DDesc {
        Size width = 0;
        Size height = 0;
        const void* data = nullptr;
        VL_TEXTURE_FORMAT format = VL_TEXTURE_FORMAT_MAX_VALUE;
        VL_BUFFER_USAGE usage = VL_BUFFER_USAGE_DEFAULT;
        bool generateMipmap = true;
    };

    struct VL_API Texture2DImageDesc {
        View<Image::IImage> image;
        VL_BUFFER_USAGE usage = VL_BUFFER_USAGE_DEFAULT;
        bool generateMipmap = true;
    };

    class VL_API Texture2D {
    public:
        Texture2D(const Texture2DDesc& desc, const Device& device):
            m_Device(device),
            m_Width(desc.width),
            m_Height(desc.height),
            m_Format(desc.format),
            m_Usage(desc.usage) {
        }

        Texture2D(const Texture2DImageDesc& desc, const Device& device):
            m_Device(device),
            m_Width(desc.image->getWidth()),
            m_Height(desc.image->getHeight()),
            m_Format(getTextureFormat(desc.image)),
            m_Usage(desc.usage) {
        }

        virtual ~Texture2D() = default;

        virtual void bind() const = 0;

        virtual void bindShaderResource(U32 slot) const = 0;

        virtual void setData(const void* data, U32 x, U32 y, U32 width, U32 height) = 0;

        virtual void copyFrom(const View<Texture2D>& other) = 0;

        virtual UP<Image::IImage> getData() const = 0;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        [[nodiscard]] Size getWidth() const { return m_Width; }

        [[nodiscard]] Size getHeight() const { return m_Height; }

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const { return m_Format; }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const { return m_Usage; }

    private:
        const Device& m_Device;

        const Size m_Width;
        const Size m_Height;
        const VL_TEXTURE_FORMAT m_Format;
        const VL_BUFFER_USAGE m_Usage;
    };

}
