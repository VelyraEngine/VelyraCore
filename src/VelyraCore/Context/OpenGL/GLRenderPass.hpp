#pragma once

#include <VelyraCore/Context/RenderPass.hpp>

namespace Velyra::Core {

    class GLRenderPass: public RenderPass {
    public:
        GLRenderPass(const View<RenderPassLayout>& layout, const Device& device);

        ~GLRenderPass() override;

        void begin() override;

        void end() override;

        void clear() override;

        void clear(Size attachmentIndex) override;

        void onResize(Size width, Size height) override;

        void onResize(Size width, Size height, Size attachmentIndex) override;

    private:

        void createColorAttachments(const View<RenderPassLayout>& layout, const Device& device);

        GLenum checkFrameBufferStatus();

    private:
        const Utils::LogPtr m_Logger = nullptr;
        U32 m_FrameBufferID = 0;
    };

}
