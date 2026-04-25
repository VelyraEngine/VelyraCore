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

    struct VL_API FrameBufferDepthStencilStateDesc {
        bool enableDepthTest                    = false;
        VL_COMPARISON_FUNC depthComparisonFunc  = VL_COMPARISON_FUNC_LESS;
        VL_DEPTH_MASK depthMask                 = VL_DEPTH_MASK_ENABLED;

        bool enableStencilTest                  = false;
        VL_COMPARISON_FUNC stencilComparisonFunc= VL_COMPARISON_FUNC_ALWAYS;
        U32 stencilMask                         = 0xFFFFFFFF;
        VL_STENCIL_FUNC stencilFail             = VL_STENCIL_FUNC_KEEP;
        VL_STENCIL_FUNC stencilPass             = VL_STENCIL_FUNC_KEEP;
        VL_STENCIL_FUNC stencilPassDepthFail    = VL_STENCIL_FUNC_KEEP;
    };

    struct VL_API FrameBufferDepthStencilDesc {
        FrameBufferDepthStencilAttachmentDesc resource = {};
        FrameBufferDepthStencilStateDesc state = {};
    };

    class VL_API FrameBufferLayout {
    public:
        explicit FrameBufferLayout(const Device& device);

        ~FrameBufferLayout();

        void setDimensions(Size width, Size height);

        void addColorAttachment(const FrameBufferColorAttachmentDesc& desc);

        void setDepthStencilAttachment(const FrameBufferDepthStencilDesc& desc);

        [[nodiscard]] bool isValid() const;

        [[nodiscard]] const std::vector<FrameBufferColorAttachmentDesc>& getColorAttachments() const;

        [[nodiscard]] const std::optional<FrameBufferDepthStencilDesc>& getDepthStencilAttachment() const;

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

        std::optional<FrameBufferDepthStencilDesc> m_DepthStencilAttachment;
        std::vector<FrameBufferColorAttachmentDesc> m_ColorAttachments;
    };

}
