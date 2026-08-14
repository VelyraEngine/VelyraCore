#include "../Pch.hpp"

#include <VelyraCore/Context/Context.hpp>
#include <VelyraCore/Context/Device.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>
#include <VelyraCore/Context/Viewport.hpp>
#include <VelyraCore/Context/Shader.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>
#include <VelyraCore/Context/MeshBinding.hpp>
#include <VelyraCore/Context/ConstantBuffer.hpp>
#include <VelyraCore/Context/Sampler.hpp>
#include <VelyraCore/Context/Texture2D.hpp>
#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include <VelyraCore/Context/DepthStencilAttachment.hpp>
#include <VelyraCore/Context/ColorAttachment.hpp>
#include <VelyraCore/Context/DepthStencilState.hpp>
#include <VelyraCore/Context/ApiState.hpp>
#include <VelyraCore/Context/StructuredBuffer.hpp>

#include "../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    Context::~Context() = default;

    void Context::createImGuiContext(const ImGuiContextDesc &desc) {
        VL_PRECONDITION(!m_ImGuiEnabled, "ImGui context already initialized!");

        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_CTX);

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
        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_CTX);

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

    Context::Context(const VL_GRAPHICS_API type): m_Type(type) {}

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


