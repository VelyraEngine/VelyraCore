#pragma once

#include "SandBoxInclude.hpp"
#include "IProcedure.hpp"

#include <unordered_set>

namespace Velyra::SandBox {

    using ProcedureGraph = std::unordered_map<VL_SBX_PROCEDURE_TYPE, std::vector<VL_SBX_PROCEDURE_TYPE>>;
    using InDegreeMap =  std::unordered_map<VL_SBX_PROCEDURE_TYPE, Size>;
    using ProcedureSet = std::unordered_set<VL_SBX_PROCEDURE_TYPE>;

    class ProcedureExecutor {
    public:
        ProcedureExecutor();

        ~ProcedureExecutor() = default;

        void addProcedure(VL_SBX_PROCEDURE_TYPE procedureType);

        void constructStrategy();

        void onUpdate(Duration deltaTime, const UP<Core::Context>& context, const UP<Core::Window>& window) const;

        void onEvent(const Core::Event& event, const UP<Core::Context>& context, const UP<Core::Window>& window) const;

        void onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) const;

    private:

        template<typename PROCEDURE_FACTORY>
        void registerProcedureFactory() {
            static_assert(std::is_base_of_v<IProcedureFactory, PROCEDURE_FACTORY>, "PROCEDURE_FACTORY type is not derived from IProcedureFactory");

            auto factory = createSP<PROCEDURE_FACTORY>();
            m_ProcedureFactories.insert({factory->provides_procedure(), factory});
        }

        void prepareConstructStrategy();

        void collectRequiredProcedures(ProcedureSet& requiredProcedures) const;

        void constructProcedureGrap(VL_SBX_PROCEDURE_TYPE procedure, ProcedureSet& visited, ProcedureGraph& graph, InDegreeMap& inDegree) const;

        /**
         * @brief Kahn's algorithm for topological sorting
         * @param graph
         * @param inDegree
         * @return
         */
        std::vector<VL_SBX_PROCEDURE_TYPE> topologicalSort(const ProcedureGraph& graph, InDegreeMap& inDegree) const;

    private:
        Utils::LogPtr m_Logger = Utils::getLogger(VL_SANDBOX_PROCEDURE_EXECUTOR_LOGGER);
        std::unordered_map<VL_SBX_PROCEDURE_TYPE, SP<IProcedureFactory>> m_ProcedureFactories;

        std::unordered_set<VL_SBX_PROCEDURE_TYPE> m_RequiredProcedures;

        std::vector<SP<IProcedure>> m_ProceduresOrder;
    };
}