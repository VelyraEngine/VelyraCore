#include "TexturedMVPProcedure.hpp"

namespace Velyra::SandBox {

    void TexturedMVPProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "MVP", "Texture");
    }

    void TexturedMVPProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram.shaderProgram->bind();
    }

}