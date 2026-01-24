#include "IndexDrawProcedure.hpp"

namespace Velyra::SandBox {

    static constexpr Vertex s_IndexDrawRectangle[] = {
        // Positions            // Colors             // Normals           // Tangents          // TexCoords}
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
    };

    static constexpr U32 s_IndexDrawRectangleIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    static constexpr U32 s_IndexDrawLowerLeftTriangle[] = {
        0, 1, 2, 0, 1, 2
    };

    static constexpr U32 s_IndexDrawUpperLeftTriangle[] = {
        1, 2, 3, 1, 2, 3
    };

    static constexpr U32 s_IndexDrawUpperRightTriangle[] = {
        2, 3, 0, 2, 3, 0
    };

    static constexpr U32 s_IndexDrawLowerRightTriangle[] = {
        3, 0, 1, 3, 0, 1
    };

    IndexDrawProcedure::IndexDrawProcedure() {

    }

    void IndexDrawProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        // We use the same shader as VertexDrawProcedure since they share the same shader code
        m_ShaderProgram = loadShader(context, "VertexDraw", "VertexDraw");
        auto vertexLayout = Vertex::getVertexLayout(context);

        Core::VertexBufferDesc vboDesc;
        vboDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
        vboDesc.layout = vertexLayout;
        vboDesc.count = 4;
        vboDesc.data = s_IndexDrawRectangle;
        m_VertexBuffer = context->createVertexBuffer(vboDesc);

        Core::IndexBufferDesc iboDesc;
        iboDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
        iboDesc.data = s_IndexDrawRectangleIndices;
        iboDesc.count = 6;
        iboDesc.dataType = VL_UINT32;
        m_IndexBuffer = context->createIndexBuffer(iboDesc);

        Core::MeshBindingDesc mbDesc;
        mbDesc.drawMode = VL_DRAW_TRIANGLES;
        mbDesc.vertexBuffer = m_VertexBuffer;
        mbDesc.indexBuffer = m_IndexBuffer;
        m_MeshBinding = context->createMeshBinding(mbDesc);
    }

    void IndexDrawProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram.shaderProgram->bind();
        m_MeshBinding->draw();
    }

    void IndexDrawProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Index Draw");
        ImGui::TextWrapped("This procedure draws a triangle using a Vertex Buffer and an Index Buffer).");
        if (ImGui::Button("Next Mesh")) {
            m_MeshIndex++;
            switch (m_MeshIndex % 5) {
                case 0: m_IndexBuffer->setData(0, s_IndexDrawRectangleIndices, 6);      break;
                case 1: m_IndexBuffer->setData(0, s_IndexDrawLowerLeftTriangle, 6);     break;
                case 2: m_IndexBuffer->setData(0, s_IndexDrawUpperLeftTriangle, 6);     break;
                case 3: m_IndexBuffer->setData(0, s_IndexDrawUpperRightTriangle, 6);    break;
                case 4: m_IndexBuffer->setData(0, s_IndexDrawLowerRightTriangle, 6);    break;
                default: break;
            }
        }

        drawMeshBindingWidget(m_MeshBinding);

        ImGui::End();
    }

}
