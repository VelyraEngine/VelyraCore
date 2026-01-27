#include "ProjectionProcedure.hpp"

namespace Velyra::SandBox {

    struct ProjectionData {
        glm::mat4 projection = glm::mat4(1.0f);
    };

    ProjectionProcedure::ProjectionProcedure():
    IProcedure(){

    }

    void ProjectionProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        constexpr ProjectionData zero;
        Core::ConstantBufferDesc desc;
        desc.name = "ProjectionData";
        desc.data = &zero;
        desc.size = sizeof(ProjectionData);
        desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        desc.shaderStage = VL_SHADER_VERTEX;
        m_ProjectionBuffer = context->createConstantBuffer(desc);

        setProjection(context);
    }

    void ProjectionProcedure::onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        switch (event.type) {
            case VL_EVENT_WINDOW_RESIZED: {
                setProjection(context);
                break;
            }
            default:
                break;
        }
    }

    void ProjectionProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ProjectionBuffer->bindShaderResource(2);
    }

    void ProjectionProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Projection");

        if (ImGui::DragFloat("FOV", &m_Fov, 1.0f, 0.0f, 180.0f)){
            setProjection(context);
        }
        if (ImGui::DragFloat("ZNear", &m_Near, 0.1f, 0.0f, 100.0f)){
            setProjection(context);
        }
        if (ImGui::DragFloat("ZFar", &m_Far, 0.1f, 0.0f, 100.0f)){
            setProjection(context);
        }

        ImGui::End();
    }

    void ProjectionProcedure::setProjection(const UP<Core::Context> &context) const {
        const float width = static_cast<float>(context->getClientWidth()); // Don't use the window width but the framebuffer width
        const float height = static_cast<float>(context->getClientHeight()); // Don't use the window height but the framebuffer height

        ProjectionData data;
        data.projection = glm::perspective(glm::radians(m_Fov), width / height, m_Near, m_Far);
        m_ProjectionBuffer->setData(0, &data, sizeof(ProjectionData));
    }
}
