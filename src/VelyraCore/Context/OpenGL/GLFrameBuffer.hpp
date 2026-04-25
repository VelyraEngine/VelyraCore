#pragma once

#include <VelyraCore/Context/FrameBuffer.hpp>

namespace Velyra::Core {

    class GLFrameBuffer: public FrameBuffer {
    public:
        GLFrameBuffer(const View<FrameBufferLayout>& layout, const Device& device);

        ~GLFrameBuffer() override;

        void begin() override;

        void end() override;

        void clear() override;

        void onResize(Size width, Size height) override;

    private:

        void createColorAttachments(const View<FrameBufferLayout>& layout, const Device& device);

        void createDepthStencilAttachment(const View<FrameBufferLayout>& layout, const Device& device);

        GLenum checkFrameBufferStatus();

    private:
        const Utils::LogPtr m_Logger = nullptr;
        U32 m_FrameBufferID = 0;
    };

}
