#include "DepthStencilStateProcedure.hpp"

namespace Velyra::SandBox {

    void DepthStencilStateProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        Core::DepthStencilStateDesc desc;
        desc.enableDepthTest = true;
        m_DepthStencilState = context->createDepthStencilState(desc);
    }

    void DepthStencilStateProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_DepthStencilState->bind();
    }

    void DepthStencilStateProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Depth Stencil State");
        bool isDepthTestEnabled = m_DepthStencilState->isDepthTestEnabled();
        if (ImGui::Checkbox("Enable Depth Test", &isDepthTestEnabled)) {
            m_DepthStencilState->setDepthTestEnabled(isDepthTestEnabled);
        }

        ImGui::End();
    }
}
