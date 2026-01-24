#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class CameraProcedure : public IProcedure {
    public:
        CameraProcedure();

        ~CameraProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:

        void updateCameraData();

        void mouseMoved(float mousePosX, float mousePosY);

        void move(VL_KEYBOARD_KEY keyPressed);

    private:
        SP<Core::ConstantBuffer> m_CameraBuffer = nullptr;

        bool m_Enable = false;

        Duration m_LastDeltaTime = 0.0ms;

        glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        float m_Speed = 0.01f;
        float m_Sensitivity = 0.2f;

        float m_LastX = 0.0f;
        float m_LastY = 0.0f;
    };

}