#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API RenderPassColorAttachmentDesc {
        float clearColor[4]         = {0.2f, 0.3f, 0.8f, 1.0f}; // default clear color is a nice blue
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_RGBA_F32;
        bool enableShaderAccess     = false; // enables sampling from this attachment in shaders
    };

    struct VL_API RenderPassDepthStencilAttachmentDesc {
        float clearDepth            = 1.0f;
        U32 clearStencil            = 0;
        VL_TEXTURE_FORMAT format    = VL_TEXTURE_DEPTH_24_STENCIL_8;
        bool enableShaderAccess     = false; // enables sampling from this attachment in shaders
    };

    struct VL_API RenderPassDepthStencilStateDesc {
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

    struct VL_API RenderPassDepthStencilDesc {
        RenderPassDepthStencilAttachmentDesc resource = {};
        RenderPassDepthStencilStateDesc state = {};
    };

    class VL_API RenderPassLayout {
    public:
        RenderPassLayout(const Device& device);

        ~RenderPassLayout();

        void setDimensions(Size width, Size height);

        void addColorAttachment(const RenderPassColorAttachmentDesc& desc);

        void setDepthStencilAttachment(const RenderPassDepthStencilDesc& desc);

        bool isValid() const;

        [[nodiscard]] const std::vector<RenderPassColorAttachmentDesc>& getColorAttachments() const;

        [[nodiscard]] const std::optional<RenderPassDepthStencilDesc>& getDepthStencilAttachment() const;

    private:

        bool dimensionsValid() const;

        bool colorAttachmentsValid() const;

        bool depthStencilAttachmentValid() const;

    private:
        Utils::LogPtr m_Logger = nullptr;
        const Device& m_Device;

        Size m_Width = VL_DEFAULT_WIDTH;
        Size m_Height = VL_DEFAULT_HEIGHT;

        std::optional<RenderPassDepthStencilDesc> m_DepthStencilAttachment;
        std::vector<RenderPassColorAttachmentDesc> m_ColorAttachments;
    };

}
