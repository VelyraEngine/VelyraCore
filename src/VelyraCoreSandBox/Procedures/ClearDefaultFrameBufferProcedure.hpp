#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class ClearDefaultFrameBufferProcedure: public IProcedure {
    public:
        ClearDefaultFrameBufferProcedure() = default;

        ~ClearDefaultFrameBufferProcedure() override = default;

        void onUpdate(Duration deltaTime, const UP<Core::Context>& context, const UP<Core::Window>& window) override;
    };

    VL_DECLARE_PROCEDURE_FACTORY(ClearDefaultFrameBufferProcedure, ClearDefaultFrameBuffer, {})
}