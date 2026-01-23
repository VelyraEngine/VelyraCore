#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class EventProcedure: public IProcedure {
    public:
        EventProcedure();

        ~EventProcedure() override = default;

        void onEvent(const Core::Event &event, const UP<Core::Context>& context, const UP<Core::Window>& window) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(EventProcedure, EventPrinter, {})
}