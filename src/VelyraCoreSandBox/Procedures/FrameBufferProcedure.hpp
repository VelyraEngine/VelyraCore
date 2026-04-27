#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class ProcedureExecutor;

    class FrameBufferProcedure: public IProcedure {
    public:
        FrameBufferProcedure() = default;

        ~FrameBufferProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:

        void createFrameBuffer(const UP<Core::Context> &context);

    private:
        UP<ProcedureExecutor> m_SubProcedureExecutor;
        View<Core::FrameBufferLayout> m_FrameBufferLayout;
        View<Core::FrameBuffer> m_FrameBuffer;
        View<Core::Viewport> m_Viewport;
        View<Core::DepthStencilState> m_DepthStencilState;

        View<Core::MeshBinding> m_ScreenQuadMeshBinding;
        ShaderProgram m_ScreenQuadShader;

        Utils::Color m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f};
    };

    static const std::vector<VL_SBX_PROCEDURE_TYPE> s_FrameBufferProcedures = {
    };

    VL_DECLARE_PROCEDURE_FACTORY(FrameBufferProcedure, VL_SBX_PROCEDURE_TYPE::FrameBuffer, s_FrameBufferProcedures);

}