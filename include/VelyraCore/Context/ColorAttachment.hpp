#pragma once

#include <VelyraCore/Context/Device.hpp>
#include <VelyraUtils/Types/Color.hpp>

namespace Velyra::Core {

    struct VL_API ColorAttachmentDesc {
        Size width                  = VL_DEFAULT_WIDTH;
        Size height                 = VL_DEFAULT_HEIGHT;
        Utils::Color clearColor     = {0.2f, 0.3f, 0.8f, 1.0f};
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_RGBA_F32;
        VL_BUFFER_USAGE usage       = VL_BUFFER_USAGE_DEFAULT;
    };

    class VL_API ColorAttachment {
    public:
        ColorAttachment(const ColorAttachmentDesc& desc, const Device& device):
            m_Device(device),
            m_Width(desc.width),
            m_Height(desc.height),
            m_ClearColor(desc.clearColor),
            m_Format(desc.format) {
        }

        virtual ~ColorAttachment() = default;

        virtual void bind() const = 0;

        virtual void bindShaderResource(U32 slot) const = 0;

        virtual void clear() const = 0;

        virtual void onResize(Size width, Size height) = 0;

        [[nodiscard]] virtual UP<Image::IImage> getData() const = 0;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        [[nodiscard]] Size getWidth() const { return m_Width; }

        [[nodiscard]] Size getHeight() const { return m_Height; }

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const { return m_Format; }

        [[nodiscard]] const Utils::Color& getClearColor() const { return m_ClearColor; }

    protected:
        const Device& m_Device;

        Size m_Width;
        Size m_Height;
        Utils::Color m_ClearColor;
        VL_TEXTURE_FORMAT m_Format;
    };

}