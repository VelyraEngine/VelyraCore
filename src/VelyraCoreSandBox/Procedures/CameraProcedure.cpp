#include "CameraProcedure.hpp"

namespace Velyra::SandBox {

    struct CameraData {
        glm::mat4 view = glm::mat4(1.0f);
        glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    CameraProcedure::CameraProcedure():
    IProcedure(){

    }

    void CameraProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_LastX = static_cast<float>(window->getWidth()) / 2.0f;
        m_LastY = static_cast<float>(window->getHeight()) / 2.0f;

        constexpr CameraData zero;
        Core::ConstantBufferDesc desc;
        desc.name = "CameraData";
        desc.data = &zero;
        desc.size = sizeof(CameraData);
        desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        desc.shaderStage = VL_SHADER_VERTEX;
        m_CameraBuffer = context->createConstantBuffer(desc);

        updateCameraData();
    }

    void CameraProcedure::onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        switch (event.type) {
            case VL_EVENT_MOUSE_MOVED: {
                const float xPosition = static_cast<float>(event.mousePosX);
                const float yPosition = static_cast<float>(event.mousePosY);
                if (m_Enable) {
                    mouseMoved(xPosition, yPosition);
                }
                break;
            }
            case VL_EVENT_KEYBOARD_KEY_PRESSED: {
                const auto key = static_cast<VL_KEYBOARD_KEY>(event.keyCode);
                if (key == VL_KEY_F) {
                    m_Enable = !m_Enable;
                    if (m_Enable) {
                        window->grabMouse();
                        window->hideMouse();
                    }
                    else {
                        window->releaseMouse();
                        window->showMouse();
                    }
                }
                if (m_Enable) {
                    move(key);
                }
                break;
            }
            default: break;
        }
    }

    void CameraProcedure::onUpdate(const Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_LastDeltaTime = deltaTime;

        m_CameraBuffer->bindShaderResource(1);
    }

    void CameraProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Camera");
        if (m_Enable){
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Camera enabled");
        }
        else{
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Camera disabled");
        }
        ImGui::DragFloat("Speed", &m_Speed, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("Sensitivity", &m_Sensitivity, 0.01f, 0.0f, 10.0f);
        ImGui::End();
    }

    void CameraProcedure::updateCameraData() {
        glm::vec3 front;
        front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        front.y = glm::sin(glm::radians(m_Pitch));
        front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        CameraData data;
        data.view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        data.position = glm::vec4(m_Position, 1.0f);
        m_CameraBuffer->setData(0, &data, sizeof(CameraData));
    }

    void CameraProcedure::mouseMoved(const float mousePosX, const float mousePosY) {
        float xOffset = mousePosX - m_LastX;
        float yOffset = mousePosY - m_LastY;

        m_LastX = mousePosX;
        m_LastY = mousePosY;

        xOffset *= m_Sensitivity;
        yOffset *= m_Sensitivity;

        m_Yaw += xOffset;
        m_Pitch -= yOffset;

        if (m_Pitch > 89.0f) {
            m_Pitch = 89.0f;
        }
        if (m_Pitch < -89.0f) {
            m_Pitch = -89.0f;
        }

        updateCameraData();
    }

    void CameraProcedure::move(VL_KEYBOARD_KEY keyPressed) {
        const float velocity = static_cast<float>(m_LastDeltaTime.count()) * m_Speed;
        switch (keyPressed) {
            case VL_KEY_W:  m_Position += m_Front * velocity; break;
            case VL_KEY_S:  m_Position -= m_Front * velocity; break;
            case VL_KEY_A:  m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
            case VL_KEY_D:  m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
            case VL_KEY_SPACE:  m_Position += m_Up * velocity; break;
            case VL_KEY_LEFT_CONTROL:  m_Position -= m_Up * velocity; break;
            default: break;
        }
        updateCameraData();
    }

}
