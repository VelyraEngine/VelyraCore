#include "Pch.hpp"

#include "ImGuiWidgets.hpp"

namespace Velyra::SandBox {

    void drawMeshBindingWidget(const View<Core::MeshBinding> &meshBinding) {
        if (!VL_DRAW_MODE_detail::VL_DRAW_MODE_StructuresGenerated) {
            VL_DRAW_MODE_detail::generateStructures();
        }

        auto drawMode = meshBinding->getDrawMode();
        ImGui::Text("%s", Utils::toString(drawMode).c_str());
        if (ImGui::BeginCombo("Draw Mode", Utils::toString(drawMode).c_str())) {
            for (const auto& [mode, str]: VL_DRAW_MODE_detail::_ValueToStr) {
                if (mode == VL_DRAW_MODE_MAX_VALUE) {
                    continue;
                }
                const bool isSelected = (drawMode == mode);
                if (ImGui::Selectable(str.c_str(), isSelected)) {
                    meshBinding->setDrawMode(mode);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
}
