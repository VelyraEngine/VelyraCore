#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class DataContainer;

    class ContextInfoProcedure: public IProcedure {
    public:
        ContextInfoProcedure() = default;

        ~ContextInfoProcedure() override = default;

        void onImGui(DataContainer& dataContainer, const UP<Core::Context>& context, const UP<Core::Window>& window) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(ContextInfoProcedure, ContextInfo, {ClearDefaultFrameBuffer})
}