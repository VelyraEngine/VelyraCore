#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    class GLDevice: public Device {
    public:
        GLDevice() = default;

        ~GLDevice() override = default;

        [[nodiscard]] std::string getAPIVersion() const override;

        [[nodiscard]] std::string getDeviceName() const override;

        [[nodiscard]] std::string getDeviceVendor() const override;

        [[nodiscard]] std::string getShadingLanguageVersion() const override;

        [[nodiscard]] U32 getMaxFramebufferWidth() const override;

        [[nodiscard]] U32 getMaxFramebufferHeight() const override;

        [[nodiscard]] U32 getMaxFramebufferColorAttachments() const override;

        [[nodiscard]] U32 getMaxViewportWidth() const override;

        [[nodiscard]] U32 getMaxViewportHeight() const override;

        [[nodiscard]] U32 getMaxTextureSlots() const override;

        [[nodiscard]] U32 getMaxTextureSize() const override;

        [[nodiscard]] U32 getMaxConstantBufferSize() const override;

        [[nodiscard]] U32 getMaxConstantBufferSlots() const override;

        [[nodiscard]] U32 getMaxShaderStorageBufferSize() const override;

        [[nodiscard]] U32 getMaxShaderStorageBufferSlots() const override;

        [[nodiscard]] U32 getMaxVertexAttributes() const override;

        [[nodiscard]] bool isDoubleBuffered() const override;

        [[nodiscard]] bool isTextureFormatSupported(VL_TEXTURE_FORMAT format) const override;

        [[nodiscard]] bool isColorAttachmentFormatSupported(VL_TEXTURE_FORMAT format) const override;

        [[nodiscard]] bool isDepthStencilAttachmentFormatSupported(VL_TEXTURE_FORMAT format) const override;

    private:

        static std::string getGlConstantStr(GLenum constant);

        static U32 getGlConstantInt(GLenum constant);
    };

}