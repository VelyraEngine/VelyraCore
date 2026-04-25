#pragma once

#include <VelyraCore/Context/DepthStencilState.hpp>

namespace Velyra::Core {

    class GLDepthStencilState : public DepthStencilState {
    public:
        GLDepthStencilState(const DepthStencilStateDesc& desc, const Device& device);

        ~GLDepthStencilState() override;

        void bind() const override;

        void unbind() const override;

        void setDepthTestEnabled(bool enabled) override;

        void setDepthComparisonFunc(VL_COMPARISON_FUNC func) override;

        void setDepthMask(VL_DEPTH_MASK mask) override;

        void setStencilTestEnabled(bool enabled) override;

        void setStencilComparisonFunc(VL_COMPARISON_FUNC func) override;

        void setStencilMask(U32 mask) override;

        void setStencilFailOp(VL_STENCIL_FUNC func) override;

        void setStencilPassOp(VL_STENCIL_FUNC func) override;

        void setStencilPassDepthFailOp(VL_STENCIL_FUNC func) override;

    private:
        const Utils::LogPtr m_Logger;

        GLenum m_GLDepthComparisonFunc;

        GLenum m_GLStencilComparisonFunc;
        GLenum m_GLStencilFail;
        GLenum m_GLStencilPassDepthFail;
        GLenum m_GLStencilPass;
    };

}