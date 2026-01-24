#include "VertexDrawProcedure.hpp"

namespace Velyra::SandBox {

    static constexpr Vertex s_VertexDrawRedTriangle[] = {
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.5f, 1.0f),
        glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
    };

    static constexpr Vertex s_VertexDrawGreenTriangle[] = {
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.5f, 1.0f),
        glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
    };

    VertexDrawProcedure::VertexDrawProcedure() {

    }

    void VertexDrawProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "VertexDraw", "VertexDraw");
        auto vertexLayout = Vertex::getVertexLayout(context);

        Core::VertexBufferDesc vboDesc;
        vboDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
        vboDesc.layout = vertexLayout;
        vboDesc.count = 3;
        if (m_DrawRedTriangle) {
            vboDesc.data = s_VertexDrawRedTriangle;
        }
        else {
            vboDesc.data = s_VertexDrawGreenTriangle;
        }
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
        ImGui::Begin("Vertex Draw");
        ImGui::Text("This procedure draws a triangle using a Vertex Buffer (no Index Buffer).");
        if (ImGui::Checkbox("Draw Red Triangle", &m_DrawRedTriangle)){
            if (m_DrawRedTriangle){
                m_VertexBuffer->setData(0, s_VertexDrawRedTriangle, 3);
            }
            else{
                m_VertexBuffer->setData(0, s_VertexDrawGreenTriangle, 3);
            }
        }
        if (ImGui::Button("Read Vertex Buffer Data")) {
            m_BufferData.clear();
            auto pData = m_VertexBuffer->getData();
            auto vertexData = reinterpret_cast<const Vertex*>(pData.data());
            for (Size i = 0; i < m_VertexBuffer->getCount(); i++) {
                const auto&[position, color, normal, tangent, texCoord] = vertexData[i];
                m_BufferData += "Vertex " + std::to_string(i) + ":\n";
                m_BufferData += "  Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")\n";
                m_BufferData += "  Color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")\n";
                m_BufferData += "  Normal: (" + std::to_string(normal.x) + ", " + std::to_string(normal.y) + ", " + std::to_string(normal.z) + ")\n";
                m_BufferData += "  Tangent: (" + std::to_string(tangent.x) + ", " + std::to_string(tangent.y) + ", " + std::to_string(tangent.z) + ")\n";
                m_BufferData += "  TexCoord: (" + std::to_string(texCoord.x) + ", " + std::to_string(texCoord.y) + ")\n";

            }
        }
        ImGui::TextUnformatted(m_BufferData.c_str());

        ImGui::End();
    }

}
