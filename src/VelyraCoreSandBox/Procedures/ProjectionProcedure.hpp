#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class ProjectionProcedure: public IProcedure {
    public:
        ProjectionProcedure();

        ~ProjectionProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:

        void setProjection(const UP<Core::Context> &context) const;

    private:
        float m_Fov = 45.0f;
        float m_Near = 0.1f;
        float m_Far = 100.0f;

        View<Core::ConstantBuffer> m_ProjectionBuffer = nullptr;
    };

    VL_DECLARE_PROCEDURE_FACTORY(ProjectionProcedure, Projection, {})
}