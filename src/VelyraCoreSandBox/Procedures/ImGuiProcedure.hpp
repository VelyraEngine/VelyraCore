#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class ImGuiProcedure: public IProcedure {
    public:
        ImGuiProcedure() = default;

        ~ImGuiProcedure() override = default;

        void onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(ImGuiProcedure, ImGuiDemo, {ClearDefaultFrameBuffer})
}