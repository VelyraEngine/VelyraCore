#pragma once

#include <VelyraCore/Context/Device.hpp>
#include <VelyraCore/Context/FrameBuffer.hpp>

namespace Velyra::Core::GLES {

    class GLESDefaultFrameBuffer: public FrameBuffer {
    public:
        GLESDefaultFrameBuffer(const DefaultFrameBufferDesc& desc, const Device& device);

        ~GLESDefaultFrameBuffer() override;

        void begin() override;

        void end() override;

        void clear() override;

        void onResize(Size width, Size height) override;

    private:
        const Utils::LogPtr m_Logger = nullptr;
    };

}
