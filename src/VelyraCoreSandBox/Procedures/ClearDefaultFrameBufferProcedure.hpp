#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class DataContainer;

    class ClearDefaultFrameBufferProcedure: public IProcedure {
    public:
        ClearDefaultFrameBufferProcedure() = default;

        ~ClearDefaultFrameBufferProcedure() override = default;

        void onUpdate(DataContainer &container, Duration deltaTime, const UP<Core::Context> &context) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(ClearDefaultFrameBufferProcedure, ClearDefaultFrameBuffer, {})
}