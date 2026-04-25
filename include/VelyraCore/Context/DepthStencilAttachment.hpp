#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API DepthStencilAttachmentDesc {
        Size width                  = VL_DEFAULT_WIDTH;
        Size height                 = VL_DEFAULT_HEIGHT;
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_DEPTH_24_STENCIL_8;
        float clearDepth            = 1.0f;
        U32 clearStencil            = 0;
        bool enableShaderAccess     = false; // enables sampling from this attachment in shaders
    };

    class VL_API DepthStencilAttachment {
    public:
        DepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Device& device):
        m_Device(device),
        m_Width(desc.width),
        m_Height(desc.height),
        m_Format(desc.format),
        m_ClearDepth(desc.clearDepth),
        m_ClearStencil(desc.clearStencil) {
        }

        virtual ~DepthStencilAttachment() = default;

        virtual void bind() const = 0;

        virtual void bindShaderResource(U32 slot) const = 0;

        virtual void clear() const = 0;

        virtual void onResize(Size width, Size height) = 0;

        [[nodiscard]] virtual UP<Image::IImage> getData() const = 0;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        [[nodiscard]] Size getWidth() const { return m_Width; }

        [[nodiscard]] Size getHeight() const { return m_Height; }

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const { return m_Format; }

        [[nodiscard]] float getClearDepth() const { return m_ClearDepth; }

        [[nodiscard]] U32 getClearStencil() const { return m_ClearStencil; }

    protected:
        const Device& m_Device;

        Size m_Width;
        Size m_Height;
        VL_TEXTURE_FORMAT m_Format;
        float m_ClearDepth;
        U32 m_ClearStencil;

    };

}
