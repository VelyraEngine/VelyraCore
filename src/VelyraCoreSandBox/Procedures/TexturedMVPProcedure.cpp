#include "TexturedMVPProcedure.hpp"

namespace Velyra::SandBox {

    void TexturedMVPProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "MVP", "Texture");
    }

    void TexturedMVPProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        // TODO: THIS CAN NEVER WORK! DOING THIS PROCEDURE LIKE THIS CAUSES A ONE FRAME DELAY
        // THe draw call is before the shader is bound u retardo banardo
        m_ShaderProgram.shaderProgram->bind();
    }

}