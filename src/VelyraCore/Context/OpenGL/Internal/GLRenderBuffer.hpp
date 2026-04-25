#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct GLRenderBufferDesc {
        Size width = 0;
        Size height = 0;
        VL_TEXTURE_FORMAT format = VL_TEXTURE_FORMAT_MAX_VALUE;
    };

    class GLRenderBuffer {
    public:
        GLRenderBuffer(const GLRenderBufferDesc& desc, const Device& device);

        ~GLRenderBuffer();

        void onResize(Size width, Size height);

        void bind() const;

        [[nodiscard]] GLuint getRenderBufferID() const {
            return m_RenderBufferID;
        }

        [[nodiscard]] Size getWidth() const {
            return m_Width;
        }

        [[nodiscard]] Size getHeight() const {
            return m_Height;
        }

        [[nodiscard]] VL_TEXTURE_FORMAT getFormat() const {
            return m_Format;
        }

    private:
        const Utils::LogPtr m_Logger;
        const Device& m_Device;

        U32 m_RenderBufferID = 0;
        Size m_Width = 0;
        Size m_Height = 0;
        VL_TEXTURE_FORMAT m_Format = VL_TEXTURE_FORMAT_MAX_VALUE;
    };

}
