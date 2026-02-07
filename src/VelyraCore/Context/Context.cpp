#include "../Pch.hpp"

#include <VelyraCore/Context/Context.hpp>
#include "../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    void Context::createImGuiContext(const ImGuiContextDesc &desc) {
        VL_PRECONDITION(!m_ImGuiEnabled, "ImGui context already initialized!");

        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_CONTEXT);

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
            SPDLOG_LOGGER_INFO(logger, "Enabling ImGui docking.");
        }
        if (desc.useViewports) {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
            SPDLOG_LOGGER_INFO(logger, "Enabling ImGui viewports.");
        }

        imGuiSetStyle(desc.style);
        m_ImGuiEnabled = true;
    }

    void Context::checkImGuiFlags() const {
        const ImGuiIO& io = ImGui::GetIO();
        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_CONTEXT);

        if (m_ImGuiDesc.useViewports) {
            if (io.BackendFlags & ImGuiBackendFlags_PlatformHasViewports) {
                SPDLOG_LOGGER_INFO(logger, "ImGui platform viewports enabled.");
            }
            else {
                SPDLOG_LOGGER_WARN(logger, "ImGui platform viewports could not be enabled!");
            }

            if (io.BackendFlags & ImGuiBackendFlags_RendererHasViewports) {
                SPDLOG_LOGGER_INFO(logger, "ImGui renderer viewports enabled.");
            }
            else {
                SPDLOG_LOGGER_WARN(logger, "ImGui renderer viewports could not be enabled!");
            }
        }
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


