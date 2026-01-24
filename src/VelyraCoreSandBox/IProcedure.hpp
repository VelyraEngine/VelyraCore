#pragma once

#include "DataTypes.hpp"
#include "Utils.hpp"
#include "ImGuiWidgets.hpp"

namespace Velyra::SandBox {

    class IProcedure {
    public:
        virtual ~IProcedure() = default;

        virtual void onAttach(const UP<Core::Context>& context, const UP<Core::Window>& window) {}

        virtual void onDetach(const UP<Core::Context>& context, const UP<Core::Window>& window) {}

        virtual void onUpdate(Duration deltaTime, const UP<Core::Context>& context, const UP<Core::Window>& window) {}

        virtual void onEvent(const Core::Event& event, const UP<Core::Context>& context, const UP<Core::Window>& window) {}

        virtual void onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) {}
    };

    class IProcedureFactory {
    public:
        IProcedureFactory() = default;

        virtual ~IProcedureFactory() = default;

        virtual SP<IProcedure> createProcedure() = 0;

        virtual VL_SBX_PROCEDURE_TYPE provides_procedure() const = 0;

        virtual std::vector<VL_SBX_PROCEDURE_TYPE> requires_procedures() const = 0;

    };

}

#define VL_DECLARE_PROCEDURE_FACTORY(PROCEDURE_CLASS, PROCEDURE_TYPE, REQUIRES_PROCEDURES) \
    class PROCEDURE_CLASS##Factory : public IProcedureFactory { \
    public: \
        PROCEDURE_CLASS##Factory() = default; \
        ~PROCEDURE_CLASS##Factory() override = default; \
        SP<IProcedure> createProcedure() override { \
            return std::make_shared<PROCEDURE_CLASS>(); \
        } \
        VL_SBX_PROCEDURE_TYPE provides_procedure() const override { \
            return PROCEDURE_TYPE; \
        } \
        std::vector<VL_SBX_PROCEDURE_TYPE> requires_procedures() const override { \
            return REQUIRES_PROCEDURES; \
        } \
    };
