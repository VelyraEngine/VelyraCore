#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class IndexDrawProcedure : public IProcedure {
    public:
        IndexDrawProcedure();

        ~IndexDrawProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
        SP<Core::VertexBuffer> m_VertexBuffer = nullptr;
        SP<Core::IndexBuffer> m_IndexBuffer = nullptr;
        SP<Core::MeshBinding> m_MeshBinding = nullptr;
        ShaderProgram m_ShaderProgram;

        // ImGui Variables
        Size m_MeshIndex = 0;
    };

    VL_DECLARE_PROCEDURE_FACTORY(IndexDrawProcedure, IndexDraw, {ClearDefaultFrameBuffer})

}