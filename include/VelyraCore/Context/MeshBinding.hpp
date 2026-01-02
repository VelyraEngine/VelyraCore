#pragma once

#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>

namespace Velyra::Core {

    struct VL_API MeshBindingDesc {
        VL_DRAW_MODE drawMode   = VL_DRAW_TRIANGLES;
        SP<VertexBuffer> vertexBuffer;
        SP<IndexBuffer> indexBuffer;
    };

    class VL_API MeshBinding {
    public:
        explicit MeshBinding(const MeshBindingDesc& desc)
            : m_DrawMode(desc.drawMode) {}

        virtual ~MeshBinding() = default;

        /**
         * @brief Binds the VertexBuffer and potential index buffer to the pipeline and draws them.
         */
        virtual void draw() const = 0;

        /**
         * @bruef Draws multiple instances of the bound mesh.
         * @param instanceCount Amount of instances to draw.
         */
        virtual void drawInstanced(U64 instanceCount) const = 0;

        /**
         * @brief Sets the draw mode for this mesh.
         * @param drawMode
         */
        virtual void setDrawMode(VL_DRAW_MODE drawMode) = 0;

        [[nodiscard]] VL_DRAW_MODE getDrawMode() const {
            return m_DrawMode;
        }

    protected:
        VL_DRAW_MODE m_DrawMode;
    };
}
