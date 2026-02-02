#pragma once

#include <VelyraCore/Context/MeshBinding.hpp>

namespace Velyra::Core {

    class GLArrayMeshBinding : public MeshBinding {
    public:
        explicit GLArrayMeshBinding(const MeshBindingDesc& desc);

        ~GLArrayMeshBinding() override;

        void draw() const override;

        void drawInstanced(U64 instanceCount) const override;

        void setDrawMode(VL_DRAW_MODE drawMode) override;

    protected:
        const Utils::LogPtr m_Logger;
        GLuint m_ArrayID;
        GLenum m_GlDrawMode;

        View<VertexBuffer> m_VertexBuffer;
    };

    class GLIndexedMeshBinding: public GLArrayMeshBinding {
    public:
        GLIndexedMeshBinding(const MeshBindingDesc& desc);

        ~GLIndexedMeshBinding() override = default;

        void draw() const override;

        void drawInstanced(U64 instanceCount) const override;

    protected:
        View<IndexBuffer> m_IndexBuffer;
        GLenum m_IndexType;
    };

}