#pragma once

#include <VelyraCore/Context/Device.hpp>
#include <VelyraUtils/Types/Color.hpp>

namespace Velyra::Core {

    struct VL_API FrameBufferColorAttachmentDesc {
        Utils::Color clearColor     = {0.2f, 0.3f, 0.8f, 1.0f}; // default clear color is a nice blue
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_RGBA_F32;
        bool enableShaderAccess     = false; // enables sampling from this attachment in shaders
        VL_BUFFER_USAGE usage       = VL_BUFFER_USAGE_DEFAULT;
    };

    struct VL_API FrameBufferDepthStencilAttachmentDesc {
        float clearDepth            = 1.0f;
        U32 clearStencil            = 0;
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_DEPTH_24_STENCIL_8;
        bool enableShaderAccess     = false; // enables sampling from this attachment in shaders
        VL_BUFFER_USAGE usage       = VL_BUFFER_USAGE_DEFAULT;
    };

    class VL_API FrameBufferLayout {
    public:
        explicit FrameBufferLayout(const Device& device);

        ~FrameBufferLayout();

        void setDimensions(Size width, Size height);

        void addColorAttachment(const FrameBufferColorAttachmentDesc& desc);

        void setDepthStencilAttachment(const FrameBufferDepthStencilAttachmentDesc& desc);

        [[nodiscard]] bool isValid() const;

        [[nodiscard]] const std::vector<FrameBufferColorAttachmentDesc>& getColorAttachments() const;

        [[nodiscard]] const std::optional<FrameBufferDepthStencilAttachmentDesc>& getDepthStencilAttachment() const;

        [[nodiscard]] Size getWidth() const;

        [[nodiscard]] Size getHeight() const;

    private:

        [[nodiscard]] bool dimensionsValid() const;

        [[nodiscard]] bool colorAttachmentsValid() const;

        [[nodiscard]] bool depthStencilAttachmentValid() const;

    private:
        Utils::LogPtr m_Logger = nullptr;
        const Device& m_Device;

        Size m_Width = VL_DEFAULT_WIDTH;
        Size m_Height = VL_DEFAULT_HEIGHT;

        std::optional<FrameBufferDepthStencilAttachmentDesc> m_DepthStencilAttachment;
        std::vector<FrameBufferColorAttachmentDesc> m_ColorAttachments;
    };

}
