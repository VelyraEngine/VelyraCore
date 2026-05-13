#include "MeshBindingProcedure.hpp"

#include "../Meshes.hpp"

#include <ranges>

namespace Velyra::SandBox {

    void MeshBindingProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        createTransformBuffer(context);
        createMeshBindings(context);

        if (!VL_DRAW_MODE_detail::VL_DRAW_MODE_StructuresGenerated) {
            VL_DRAW_MODE_detail::generateStructures();
        }
        if (!MeshType_detail::MeshType_StructuresGenerated) {
            MeshType_detail::generateStructures();
        }
    }

    void MeshBindingProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_TransformBuffer->bindShaderResource(0);
        m_MeshBindings[m_SelectedMeshType]->draw();
    }

    void MeshBindingProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Mesh");
        imGuiDrawModeSelector();
        imGuiMeshSelector();
        imGuiDrawTransform();
        ImGui::End();
    }

    void MeshBindingProcedure::createTransformBuffer(const UP<Core::Context> &context) {
        constexpr auto zero = glm::mat4(1.0f);

        Core::ConstantBufferDesc desc;
        desc.name = "TransformData";
        desc.data = &zero;
        desc.size = sizeof( glm::mat4);
        desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        desc.shaderStage = VL_SHADER_VERTEX;
        m_TransformBuffer = context->createConstantBuffer(desc);
    }

    void MeshBindingProcedure::createMeshBindings(const UP<Core::Context> &context) {
        m_MeshBindings[Rectangle] = loadMesh(createRectangle(0.75f), context);
        m_MeshBindings[Cube] = loadMesh(createCube(), context);
        m_MeshBindings[Sphere] = loadMesh(createSphere(16, 16), context);
        m_MeshBindings[Cone] = loadMesh(createCone(16, 1.0f, 1.0f), context);
        m_MeshBindings[Cylinder] = loadMesh(createCylinder(16, 1.0f, 1.0f), context);
        m_MeshBindings[Torus] = loadMesh(createTorus(16, 16, 1.0f, 0.5f), context);
        m_MeshBindings[Plane] = loadMesh(createPlane(1.0f), context);
    }

    void MeshBindingProcedure::imGuiDrawModeSelector() {
        if (ImGui::BeginCombo("Draw Mode", Utils::toString(m_SelectedDrawMode).c_str())) {
            for (const auto& [str, val]: VL_DRAW_MODE_detail::_StrToValue) {
                const bool isSelected = (m_SelectedDrawMode == val);
                if (ImGui::Selectable(str.c_str(), isSelected)) {
                    m_SelectedDrawMode = val;
                    for (const auto &meshBinding: m_MeshBindings | std::views::values) {
                        meshBinding->setDrawMode(m_SelectedDrawMode);
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    void MeshBindingProcedure::imGuiMeshSelector() {
        if (ImGui::BeginCombo("Mesh", Utils::toString(m_SelectedMeshType).c_str())) {
            for (const auto& [str, val]: MeshType_detail::_StrToValue) {
                const bool isSelected = (m_SelectedMeshType == val);
                if (ImGui::Selectable(str.c_str(), isSelected)) {
                    m_SelectedMeshType = val;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    void MeshBindingProcedure::imGuiDrawTransform() {
        if (ImGui::DragFloat3("Translate", &m_InputTranslate.x, 0.01f)) {
            setTransformData();
        }
        if (ImGui::DragFloat3("Rotate", &m_InputRotate.x, 0.01f)) {
            setTransformData();
        }
        if (ImGui::DragFloat3("Scale", &m_InputScale.x, 0.01f)) {
            setTransformData();
        }

    }

    void MeshBindingProcedure::setTransformData() {
        const glm::mat4 translate  = glm::translate(glm::mat4(1.0f), m_InputTranslate);
        const glm::mat4 rotate = glm::toMat4(glm::quat(m_InputRotate));
        const glm::mat4 scale = glm::scale(m_InputScale);

        const glm::mat4 transform = translate * scale * rotate;
        m_TransformBuffer->setData(0, &transform, sizeof(glm::mat4));
    }
}
