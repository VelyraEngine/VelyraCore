#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class TexturedMVPProcedure: public IProcedure {
    public:
        TexturedMVPProcedure() = default;

        ~TexturedMVPProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
        ShaderProgram m_ShaderProgram;
    };

    static const std::vector<VL_SBX_PROCEDURE_TYPE> s_TexturedMVPProcedures = {
        ClearDefaultFrameBuffer, Camera, Projection, VL_SBX_PROCEDURE_TYPE::MeshBinding, VL_SBX_PROCEDURE_TYPE::Texture2D
    };

    VL_DECLARE_PROCEDURE_FACTORY(TexturedMVPProcedure, TexturedMVP, s_TexturedMVPProcedures);

}