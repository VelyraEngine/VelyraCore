#pragma once

#include <VelyraCore/Context/Viewport.hpp>

namespace Velyra::Core {

    class GLViewport : public Viewport {
    public:
        explicit GLViewport(const ViewportDesc& desc);

        ~GLViewport() override = default;

        void bind() override;

        void resize(U32 width, U32 height) override;

        void setPosition(U32 x, U32 y) override;

        void setDepthRange(float minDepth, float maxDepth) override;

        [[nodiscard]] U32 getWidth() const override;

        [[nodiscard]] U32 getHeight() const override;

        [[nodiscard]] U32 getXPos() const override;

        [[nodiscard]] U32 getYPos() const override;

        [[nodiscard]] float getMinDepth() const override;

        [[nodiscard]] float getMaxDepth() const override;

    private:
        const Utils::LogPtr m_Logger;

        U32 m_Width;
        U32 m_Height;
        U32 m_XPos;
        U32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;
    };

}