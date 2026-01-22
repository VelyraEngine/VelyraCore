#pragma once

#include "SandBoxInclude.hpp"
#include "DataTypes.hpp"

namespace Velyra::SandBox {

    class DataContainer;

    class IProcedure {
    public:
        virtual ~IProcedure() = default;

        virtual void onUpdate(DataContainer& container, Duration deltaTime, const UP<Core::Context>& context) {}

        virtual void onEvent(DataContainer& container,const Core::Event& event) {}

        virtual void onImGui(DataContainer& container, const UP<Core::Context>& context, const UP<Core::Window>& window) {}
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
