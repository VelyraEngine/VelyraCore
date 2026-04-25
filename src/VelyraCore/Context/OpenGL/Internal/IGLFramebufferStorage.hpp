#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraImage/IImage.hpp>

namespace Velyra::Core {

    class IGLFramebufferStorage {
    public:
        virtual ~IGLFramebufferStorage() = default;

        virtual void attachToFramebuffer(U32 framebufferID, GLenum attachmentPoint) = 0;

        virtual void onResize(Size width, Size height) = 0;

        virtual void bind() const = 0;

        [[nodiscard]] virtual U64 getID() const = 0;

        [[nodiscard]] virtual Size getWidth() const = 0;

        [[nodiscard]] virtual Size getHeight() const = 0;

        [[nodiscard]] virtual VL_TEXTURE_FORMAT getFormat() const = 0;

        // Capability checks
        [[nodiscard]] virtual bool supportsShaderAccess() const = 0;

        [[nodiscard]] virtual bool supportsReadback() const = 0;

        // Optional operations - only valid if capabilities are supported
        virtual void bindShaderResource(U32 slot) const = 0;

        [[nodiscard]] virtual UP<Image::IImage> getData() const = 0;
    };

}
