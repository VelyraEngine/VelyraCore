#pragma once

#include "Definitions.hpp"

namespace Velyra::Core {

    struct VL_API ViewportDesc{
        U32 width       = VL_DEFAULT_WIDTH;
        U32 height      = VL_DEFAULT_HEIGHT;
        U32 xPosition   = 0;
        U32 yPosition   = 0;
        float minDepth  = 0.0f;
        float maxDepth  = 1.0f;
    };

    class VL_API Viewport {
    public:
        Viewport() = default;

        virtual ~Viewport() = default;

        virtual void bind() = 0;

        virtual void resize(U32 width, U32 height) = 0;

        virtual void setPosition(U32 x, U32 y) = 0;

        virtual void setDepthRange(float minDepth, float maxDepth) = 0;

        [[nodiscard]] virtual U32 getWidth() const = 0;

        [[nodiscard]] virtual U32 getHeight() const = 0;

        [[nodiscard]] virtual U32 getXPos() const = 0;

        [[nodiscard]] virtual U32 getYPos() const = 0;

        [[nodiscard]] virtual float getMinDepth() const = 0;

        [[nodiscard]] virtual float getMaxDepth() const = 0;
    };
}