#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class VertexDrawProcedure : public IProcedure {
    public:
        VertexDrawProcedure();

        ~VertexDrawProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
        View<Core::VertexBuffer> m_VertexBuffer = nullptr;
        View<Core::MeshBinding> m_MeshBinding = nullptr;
        ShaderProgram m_ShaderProgram;

        // ImGui Variables
        bool m_DrawRedTriangle = true;
        std::string m_BufferData;
    };

    VL_DECLARE_PROCEDURE_FACTORY(VertexDrawProcedure, VertexDraw, {ClearDefaultFrameBuffer})

}