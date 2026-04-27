#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class MVPProcedure: public IProcedure {
    public:
        MVPProcedure();

        ~MVPProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
        ShaderProgram m_ShaderProgram;
    };

    static const std::vector<VL_SBX_PROCEDURE_TYPE> s_ModelViewProjectionProcedures = {
        Camera, Projection, VL_SBX_PROCEDURE_TYPE::MeshBinding
    };

    VL_DECLARE_PROCEDURE_FACTORY(MVPProcedure, MVP, s_ModelViewProjectionProcedures);

}