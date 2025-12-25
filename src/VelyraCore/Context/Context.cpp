#include "../Pch.hpp"

#include <VelyraCore/Context/Context.hpp>

namespace Velyra::Core {

    void Context::createImGuiContext(const ImGuiContextDesc &desc) {
        VL_PRECONDITION(!m_ImGuiEnabled, "ImGui context already initialized!");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        if (desc.useImPlot) {
            ImPlot::CreateContext();
            m_ImPlotEnabled = true;
        }

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Enable DPI scaling (looks better on high-DPI monitors like my 4K one)
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;

        imGuiSetStyle(desc.style);
        m_ImGuiEnabled = true;
    }

    void Context::DestroyImGuiContext() {
        VL_PRECONDITION(m_ImGuiEnabled, "There does not exists an ImGui context");

        if (!m_ImGuiEnabled){
            return;
        }
        if (m_ImPlotEnabled) {
            ImPlot::DestroyContext();
            m_ImPlotEnabled = false;
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


