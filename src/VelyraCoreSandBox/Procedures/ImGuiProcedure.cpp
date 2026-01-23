#include "ImGuiProcedure.hpp"

namespace Velyra::SandBox {

    void ImGuiProcedure::onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) {
        ImGui::Begin("Hello, VelyraCore!");

        ImGui::Text("This is an example of ImGui integration in VelyraCore SandBox.");
        ImGui::Text("Context API: %s", toString(context->getType()).c_str());

        ImGui::ShowDemoWindow();
        ImGui::ShowDebugLogWindow();
        if (context->isImPlotEnabled()) {
            ImPlot::ShowDemoWindow();
        }

        ImGui::End();
    }

}
