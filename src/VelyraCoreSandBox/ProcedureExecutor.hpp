#pragma once

#include "SandBoxInclude.hpp"
#include "DataContainer.hpp"
#include "IProcedure.hpp"

namespace Velyra::SandBox {

    class ProcedureExecutor {
    public:
        ProcedureExecutor();

        ~ProcedureExecutor() = default;

        void addProcedure(VL_SBX_PROCEDURE_TYPE procedureType);

        void constructStrategy();

        void onUpdate(Duration deltaTime);

        void onEvent(const Core::Event& event);

        void onImGui(const UP<Core::Context>& context);

    private:

        template<typename PROCEDURE_FACTORY>
        void registerProcedureFactory() {
            static_assert(std::is_base_of_v<IProcedureFactory, PROCEDURE_FACTORY>, "PROCEDURE_FACTORY type is not derived from IProcedureFactory");

            auto factory = createSP<PROCEDURE_FACTORY>();
            m_ProcedureFactories.insert({factory->provides_procedure(), factory});
        }

    private:
        Utils::LogPtr m_Logger = Utils::getLogger(VL_SANDBOX_PROCEDURE_EXECUTOR_LOGGER);
        DataContainer m_DataContainer;
        std::unordered_map<VL_SBX_PROCEDURE_TYPE, SP<IProcedureFactory>> m_ProcedureFactories;

        std::vector<VL_SBX_PROCEDURE_TYPE> m_RequiredProcedures;

        std::vector<SP<IProcedure>> m_ProceduresOrder;
    };
}