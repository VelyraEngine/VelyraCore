#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class ModelViewProjectionProcedure: public IProcedure {
    public:
        ModelViewProjectionProcedure();

        ~ModelViewProjectionProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
        View<Core::ConstantBuffer> m_TransformBuffer = nullptr;
        View<Core::MeshBinding> m_MeshBinding = nullptr;
        ShaderProgram m_ShaderProgram;
    };

    static const std::vector<VL_SBX_PROCEDURE_TYPE> s_ModelViewProjectionProcedures = {
        ClearDefaultFrameBuffer, Camera, Projection
    };

    VL_DECLARE_PROCEDURE_FACTORY(ModelViewProjectionProcedure, ModelViewProjection, s_ModelViewProjectionProcedures);

}