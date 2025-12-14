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

        void update();

        void renderImGui();

    private:
        Core::ContextDesc m_ContextDesc;
        Utils::LogPtr m_Logger = Utils::getLogger(VL_SANDBOX_LOGGER);
        UP<Core::Window> m_Window = nullptr;
        TimePoint m_LastFrameTime = getTime();

        ProcedureExecutor m_ProcedureExecutor;
    };

}