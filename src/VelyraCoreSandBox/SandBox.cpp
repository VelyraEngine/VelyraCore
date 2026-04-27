#include "Pch.hpp"

#include "SandBox.hpp"

namespace Velyra::SandBox {

    SandBox::SandBox(const std::vector<std::string_view> &args) {
        Core::WindowDesc windowDesc;
        windowDesc.title = "VelyraCore SandBox";

        for (Size i = 1; i < args.size(); i++) {
            const auto& arg = args[i];
            if (arg == "-width" || arg == "-w") {
                if (i + 1 < args.size()) {
                    windowDesc.width = static_cast<U32>(std::stoul(std::string(args[i + 1])));
                    i++;
                }
            }
            else if (arg == "-height" || arg == "-h") {
                if (i + 1 < args.size()) {
                    windowDesc.height = static_cast<U32>(std::stoul(std::string(args[i + 1])));
                    i++;
                }
            }
            else if (arg == "-x") {
                if (i + 1 < args.size()) {
                    windowDesc.xPosition = std::stoi(std::string(args[i + 1]));
                    i++;
                }
            }
            else if (arg == "-y") {
                if (i + 1 < args.size()) {
                    windowDesc.yPosition = std::stoi(std::string(args[i + 1]));
                    i++;
                }
            }
            else if (arg == "-api") {
                if (i + 1 < args.size()) {
                    m_ContextDesc.api = Utils::fromString<VL_GRAPHICS_API>(std::string(args[i + 1]));
                    i++;
                }
            }
            else if (arg == "-use-imgui-viewports") {
                m_ImGuiContextDesc.useViewports = true;
            }
            else {
                const auto argStr = std::string(arg);
                const VL_SBX_PROCEDURE_TYPE procedureType = Utils::fromString<VL_SBX_PROCEDURE_TYPE>(argStr);
                if (procedureType == VL_SBX_PROCEDURE_TYPE_MAX_VALUE) {
                    SPDLOG_LOGGER_ERROR(m_Logger, "Unknown procedure type argument: {}", argStr);
                }
                else {
                    m_ProcedureExecutor.addProcedure(procedureType);
                }
            }
        }
        m_Window = Core::WindowFactory::createWindow(windowDesc);
        
        VL_POSTCONDITION(m_Window != nullptr, "SandBox Failed to create window");
    }

    SandBox::~SandBox() {

    }

    void SandBox::run() {
        m_ContextDesc.api = VL_API_OPENGL; // TODO: Add a VL_API_BEST translation func
        const UP<Core::Context>& context = m_Window->createContext(m_ContextDesc);
        context->setVerticalSynchronisation(true);
        context->createImGuiContext(m_ImGuiContextDesc);

        m_ProcedureExecutor.constructStrategy(context, m_Window);

        while (m_Window->isOpen()) {
            processEvents(context);

            context->beginFrame();
            update(context);
            renderImGui(context);
            context->endFrame();

            context->swapBuffers();
        }

        context->DestroyImGuiContext();
    }

    void SandBox::processEvents(const UP<Core::Context>& context) const {
        m_Window->pollEvents();
        while (m_Window->hasEvent()) {
            const Core::Event event = m_Window->getNextEvent();
            if (event.type == VL_EVENT_WINDOW_CLOSED) {
                m_Window->close();
            }
            m_ProcedureExecutor.onEvent(event, context, m_Window);
        }
    }

    void SandBox::update(const UP<Core::Context>& context) {
        const TimePoint currentTime = getTime();
        const Duration deltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;
        m_ProcedureExecutor.onUpdate(deltaTime, context, m_Window);
    }

    void SandBox::renderImGui(const UP<Core::Context>& context) const {
        context->onImGuiBegin();
        m_ProcedureExecutor.onImGui(context, m_Window);
        context->onImGuiEnd();
    }

}
