#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class SamplerProcedure: public IProcedure {
    public:
        SamplerProcedure() = default;

        ~SamplerProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) override;

    private:
        View<Core::Sampler> m_Sampler = nullptr;

        int minFilterIndex = 0;
        int magFilterIndex = 0;
        int wrapUModeIndex = 0;
        int wrapVModeIndex = 0;
        int wrapWModeIndex = 0;
    };

    VL_DECLARE_PROCEDURE_FACTORY(SamplerProcedure, Sampler, {})
}