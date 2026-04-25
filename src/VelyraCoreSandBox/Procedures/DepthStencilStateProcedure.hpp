#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

namespace Velyra::SandBox {

    class DepthStencilStateProcedure : public IProcedure {
    public:
        DepthStencilStateProcedure() = default;

        ~DepthStencilStateProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:
       View<Core::DepthStencilState> m_DepthStencilState = nullptr;
    };

    VL_DECLARE_PROCEDURE_FACTORY(DepthStencilStateProcedure, VL_SBX_PROCEDURE_TYPE::DepthStencilState, {})

}