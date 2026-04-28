#pragma once

#include <VelyraCore/Context/ColorAttachment.hpp>
#include <VelyraCore/Context/DepthStencilAttachment.hpp>

namespace Velyra::Core {

    struct DefaultFrameBufferDesc {
        Utils::Color clearColor     = {0.6f, 0.8f, 0.3f, 1.0f};
        float clearDepth            = 1.0f;
        U32 clearStencil            = 0;
    };

    class VL_API FrameBuffer {
    public:
        FrameBuffer(){}

        virtual ~FrameBuffer() = default;

        virtual void begin() = 0;

        virtual void end() = 0;

        virtual void clear() = 0;

        virtual void onResize(Size width, Size height) = 0;

        [[nodiscard]] const UP<ColorAttachment>& getColorAttachment(const Size index) const {
            VL_PRECONDITION(index < m_ColorAttachments.size(), "[FrameBuffer]: Attempted to access color attachment at index {}, but only {} color attachments exist", index, m_ColorAttachments.size());

            return m_ColorAttachments.at(index);
        }

        [[nodiscard]] const UP<DepthStencilAttachment>& getDepthStencilAttachment() const {
            VL_PRECONDITION(m_DepthStencilAttachment, "[FrameBuffer]: Attempted to access depth stencil attachment, but no depth stencil attachment exists");

            return m_DepthStencilAttachment;
        }

    protected:
        std::vector<UP<ColorAttachment>> m_ColorAttachments;
        UP<DepthStencilAttachment> m_DepthStencilAttachment;

    };

}