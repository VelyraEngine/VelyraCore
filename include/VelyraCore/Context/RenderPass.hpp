#pragma once

#include <VelyraCore/Context/RenderPassLayout.hpp>
#include <VelyraCore/Context/ColorAttachment.hpp>

namespace Velyra::Core {

    class VL_API RenderPass {
    public:
        explicit RenderPass(const View<RenderPassLayout>& /*layout*/){}

        virtual ~RenderPass() = default;

        virtual void begin() = 0;

        virtual void end() = 0;

        virtual void clear() = 0;

        virtual void clear(Size attachmentIndex) = 0;

        virtual void onResize(Size width, Size height) = 0;

        virtual void onResize(Size width, Size height, Size attachmentIndex) = 0;

        [[nodiscard]] const UP<ColorAttachment>& getColorAttachment(const Size index) const {
            return m_ColorAttachments.at(index);
        }

    protected:
        std::vector<UP<ColorAttachment>> m_ColorAttachments;

    };

}