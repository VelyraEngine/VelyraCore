#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class EventProcedure: public IProcedure {
    public:
        EventProcedure();

        ~EventProcedure() override = default;

        void onEvent(DataContainer& dataContainer, const Core::Event &event) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(EventProcedure, EventPrinter, {})
}