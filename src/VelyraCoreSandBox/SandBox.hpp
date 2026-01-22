#pragma once

#include <vector>
#include <string_view>

#include "ProcedureExecutor.hpp"

namespace Velyra::SandBox {

    class SandBox {
    public:
        explicit SandBox(const std::vector<std::string_view>& args);

        ~SandBox();

        void run();

    private:

        void processEvents();

        void update(const UP<Core::Context>& context);

        void renderImGui(const UP<Core::Context>& context);

    private:
        Core::ContextDesc m_ContextDesc;
        Core::ImGuiContextDesc m_ImGuiContextDesc;
        Utils::LogPtr m_Logger = Utils::getLogger(VL_SANDBOX_LOGGER);
        UP<Core::Window> m_Window = nullptr;
        TimePoint m_LastFrameTime = getTime();

        ProcedureExecutor m_ProcedureExecutor;
    };

}