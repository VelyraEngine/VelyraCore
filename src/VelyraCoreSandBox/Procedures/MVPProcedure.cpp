#include "MVPProcedure.hpp"

namespace Velyra::SandBox {

    MVPProcedure::MVPProcedure():
    IProcedure(){

    }

    void MVPProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "MVP", "MVP");
    }

    void MVPProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram.shaderProgram->bind();
    }

}
