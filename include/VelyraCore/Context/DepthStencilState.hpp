#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API DepthStencilStateDesc {
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

    class VL_API DepthStencilState {
    public:
        explicit DepthStencilState(const DepthStencilStateDesc& desc, const Device& device):
        m_Device(device),
        m_DepthTestEnabled(desc.enableDepthTest),
        m_DepthComparisonFunc(desc.depthComparisonFunc),
        m_DepthMask(desc.depthMask),
        m_StencilTestEnabled(desc.enableStencilTest),
        m_StencilComparisonFunc(desc.stencilComparisonFunc),
        m_StencilMask(desc.stencilMask),
        m_StencilFailOp(desc.stencilFail),
        m_StencilPassOp(desc.stencilPass),
        m_StencilPassDepthFailOp(desc.stencilPassDepthFail) {
        }

        virtual ~DepthStencilState() = default;

        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void setDepthTestEnabled(bool enabled) = 0;

        virtual void setDepthComparisonFunc(VL_COMPARISON_FUNC func) = 0;

        virtual void setDepthMask(VL_DEPTH_MASK mask) = 0;

        virtual void setStencilTestEnabled(bool enabled) = 0;

        virtual void setStencilComparisonFunc(VL_COMPARISON_FUNC func) = 0;

        virtual void setStencilMask(U32 mask) = 0;

        virtual void setStencilFailOp(VL_STENCIL_FUNC func) = 0;

        virtual void setStencilPassOp(VL_STENCIL_FUNC func) = 0;

        virtual void setStencilPassDepthFailOp(VL_STENCIL_FUNC func) = 0;

        [[nodiscard]] bool isDepthTestEnabled() const { return m_DepthTestEnabled; }

        [[nodiscard]] VL_COMPARISON_FUNC getDepthComparisonFunc() const { return m_DepthComparisonFunc; }

        [[nodiscard]] VL_DEPTH_MASK getDepthMask() const { return m_DepthMask; }

        [[nodiscard]] bool isStencilTestEnabled() const { return m_StencilTestEnabled; }

        [[nodiscard]] VL_COMPARISON_FUNC getStencilComparisonFunc() const { return m_StencilComparisonFunc; }

        [[nodiscard]] U32 getStencilMask() const { return m_StencilMask; }

        [[nodiscard]] VL_STENCIL_FUNC getStencilFailOp() const { return m_StencilFailOp; }

        [[nodiscard]] VL_STENCIL_FUNC getStencilPassOp() const { return m_StencilPassOp; }

        [[nodiscard]] VL_STENCIL_FUNC getStencilPassDepthFailOp() const { return m_StencilPassDepthFailOp; }

    protected:
        const Device& m_Device;

        bool m_DepthTestEnabled;
        VL_COMPARISON_FUNC m_DepthComparisonFunc;
        VL_DEPTH_MASK m_DepthMask;

        bool m_StencilTestEnabled;
        VL_COMPARISON_FUNC m_StencilComparisonFunc;
        U32 m_StencilMask;
        VL_STENCIL_FUNC m_StencilFailOp;
        VL_STENCIL_FUNC m_StencilPassOp;
        VL_STENCIL_FUNC m_StencilPassDepthFailOp;
    };
}