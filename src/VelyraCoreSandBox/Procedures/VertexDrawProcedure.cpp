#include "VertexDrawProcedure.hpp"

namespace Velyra::SandBox {

    static const Vertex s_VertexDrawRectangle[] = {
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
        glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f),
        glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),

        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),
        glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
    };

    static const Vertex s_VertexDrawTriangle[] = {
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.5f, 1.0f),
        glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
    };

    VertexDrawProcedure::VertexDrawProcedure() {

    }

    void VertexDrawProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "VertexDraw", "VertexDraw");
        auto vertexLayout = Vertex::getVertexLayout(context);

        Core::VertexBufferDesc vboDesc;
        vboDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
        vboDesc.data = s_VertexDrawRectangle;
        vboDesc.layout = vertexLayout;
        vboDesc.count = 6;
        m_VertexBuffer = context->createVertexBuffer(vboDesc);

        Core::MeshBindingDesc mbDesc;
        mbDesc.drawMode = VL_DRAW_TRIANGLES;
        mbDesc.vertexBuffer = m_VertexBuffer;
        m_MeshBinding = context->createMeshBinding(mbDesc);
    }

    void VertexDrawProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram.shaderProgram->bind();
        m_MeshBinding->draw();
    }

    void VertexDrawProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {

    }

}
