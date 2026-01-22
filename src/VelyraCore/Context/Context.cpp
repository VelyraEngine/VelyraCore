#include "../Pch.hpp"

#include <VelyraCore/Context/Context.hpp>

namespace Velyra::Core {

    void Context::createImGuiContext(const ImGuiContextDesc &desc) {
        VL_PRECONDITION(!m_ImGuiEnabled, "ImGui context already initialized!");

        m_ImGuiDesc = desc;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        if (desc.useImPlot) {
            ImPlot::CreateContext();
        }

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Enable DPI scaling (looks better on high-DPI monitors like my 4K one)
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;

        if (desc.useDocking) {
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable docking
        }
        if (desc.useViewports) {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
        }

        imGuiSetStyle(desc.style);
        m_ImGuiEnabled = true;
    }

    void Context::DestroyImGuiContext() {
        VL_PRECONDITION(m_ImGuiEnabled, "There does not exists an ImGui context");

        if (!m_ImGuiEnabled){
            return;
        }
        if (m_ImGuiDesc.useImPlot) {
            ImPlot::DestroyContext();
        }
        ImGui::DestroyContext();
        m_ImGuiEnabled = false;
    }

    void Context::imGuiSetStyle(const VL_IMGUI_STYLE style) {
        switch (style) {
            case VL_IMGUI_STYLE_DEFAULT:
            case VL_IMGUI_STYLE_DARK:       ImGui::StyleColorsDark();   break;
            case VL_IMGUI_STYLE_CLASSIC:    ImGui::StyleColorsClassic();  break;
            case VL_IMGUI_STYLE_LIGHT:      ImGui::StyleColorsLight();  break;
            default: ImGui::StyleColorsDark();
        }
    }


}


