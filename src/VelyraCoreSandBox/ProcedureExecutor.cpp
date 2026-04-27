#include "ProcedureExecutor.hpp"

#include "Procedures/EventProcedure.hpp"
#include "Procedures/ImGuiProcedure.hpp"
#include "Procedures/ContextInfoProcedure.hpp"
#include "Procedures/FileDialogProcedure.hpp"
#include "Procedures/VertexDrawProcedure.hpp"
#include "Procedures/IndexDrawProcedure.hpp"
#include "Procedures/CameraProcedure.hpp"
#include "Procedures/ProjectionProcedure.hpp"
#include "Procedures/MVPProcedure.hpp"
#include "Procedures/SamplerProcedure.hpp"
#include "Procedures/TextureProcedure.hpp"
#include "Procedures/MeshBindingProcedure.hpp"
#include "Procedures/TexturedMVPProcedure.hpp"
#include "Procedures/DepthStencilStateProcedure.hpp"

#include <queue>

namespace Velyra::SandBox {

    ProcedureExecutor::ProcedureExecutor() {
        registerProcedureFactory<EventProcedureFactory>();
        registerProcedureFactory<ImGuiProcedureFactory>();
        registerProcedureFactory<ContextInfoProcedureFactory>();
        registerProcedureFactory<FileDialogProcedureFactory>();
        registerProcedureFactory<VertexDrawProcedureFactory>();
        registerProcedureFactory<IndexDrawProcedureFactory>();
        registerProcedureFactory<CameraProcedureFactory>();
        registerProcedureFactory<ProjectionProcedureFactory>();
        registerProcedureFactory<MVPProcedureFactory>();
        registerProcedureFactory<MeshBindingProcedureFactory>();
        registerProcedureFactory<SamplerProcedureFactory>();
        registerProcedureFactory<TextureProcedureFactory>();
        registerProcedureFactory<TexturedMVPProcedureFactory>();
        registerProcedureFactory<DepthStencilStateProcedureFactory>();
    }

    void ProcedureExecutor::addProcedure(const VL_SBX_PROCEDURE_TYPE procedureType) {
        m_RequiredProcedures.insert(procedureType);
    }

    void ProcedureExecutor::constructStrategy(const UP<Core::Context>& context, const UP<Core::Window>& window) {
        prepareConstructStrategy();
        ProcedureSet requiredProcedures = m_RequiredProcedures;
        ProcedureGraph graph;
        InDegreeMap inDegree;
        ProcedureSet visited;

        collectRequiredProcedures(requiredProcedures);
        for (const VL_SBX_PROCEDURE_TYPE procedureType: requiredProcedures) {
            constructProcedureGrap(procedureType, visited, graph, inDegree);
        }
        const auto sortedProcedures = topologicalSort(graph, inDegree);
        for (const VL_SBX_PROCEDURE_TYPE procedureType: sortedProcedures) {
            auto itFactory = m_ProcedureFactories.find(procedureType);
            if (itFactory == m_ProcedureFactories.end()) {
                SPDLOG_LOGGER_ERROR(m_Logger, "No factory found for procedure type {}", procedureType);
                continue;
            }
            const auto procedure = itFactory->second->createProcedure();
            m_ProceduresOrder.push_back(procedure);
            SPDLOG_LOGGER_INFO(m_Logger, "Added procedure type {} to execution order", procedureType);
        }

        attachProcedures(context, window);
    }

    void ProcedureExecutor::onUpdate(const Duration deltaTime, const UP<Core::Context>& context, const UP<Core::Window>& window) const {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onUpdate(deltaTime, context, window);
        }
    }

    void ProcedureExecutor::onEvent(const Core::Event &event, const UP<Core::Context>& context, const UP<Core::Window>& window) const {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onEvent(event, context, window);
        }
    }

    void ProcedureExecutor::onImGui(const UP<Core::Context> &context, const UP<Core::Window>& window) const {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onImGui(context, window);
        }
    }

    void ProcedureExecutor::prepareConstructStrategy() {
        m_ProceduresOrder.clear();
    }

    void ProcedureExecutor::collectRequiredProcedures(ProcedureSet& requiredProcedures) const {
        for (const VL_SBX_PROCEDURE_TYPE procedure: requiredProcedures) {
            auto itFactory = m_ProcedureFactories.find(procedure);
            if (itFactory == m_ProcedureFactories.end()) {
                SPDLOG_LOGGER_ERROR(m_Logger, "No factory found for procedure type {}", procedure);
                continue;
            }
            const auto& required = itFactory->second->requires_procedures();
            for (const VL_SBX_PROCEDURE_TYPE reqProcedure: required) {
                if (!requiredProcedures.contains(reqProcedure)) {
                    requiredProcedures.insert(reqProcedure);
                    SPDLOG_LOGGER_INFO(m_Logger, "Procedure type {} requires procedure type {}", procedure, reqProcedure);
                    collectRequiredProcedures(requiredProcedures);
                }
            }
        }
    }

    void ProcedureExecutor::constructProcedureGrap(const VL_SBX_PROCEDURE_TYPE procedure, ProcedureSet &visited,
        ProcedureGraph &graph, InDegreeMap &inDegree) const {
        if (visited.contains(procedure)) {
            return;
        }
        visited.insert(procedure);
        auto itFactory = m_ProcedureFactories.find(procedure);
        if (itFactory == m_ProcedureFactories.end()) {
            return; // We already logged this error in collectRequiredProcedures
        }
        inDegree.try_emplace(procedure, 0);
        const auto& requiredProcedures = itFactory->second->requires_procedures();
        for (const VL_SBX_PROCEDURE_TYPE reqProcedure: requiredProcedures) {
            graph[reqProcedure].push_back(procedure);
            inDegree[procedure]++;
            constructProcedureGrap(reqProcedure, visited, graph, inDegree);
        }
    }

    std::vector<VL_SBX_PROCEDURE_TYPE> ProcedureExecutor::topologicalSort(const ProcedureGraph &graph, InDegreeMap &inDegree) const {
        std::queue<VL_SBX_PROCEDURE_TYPE> queue;

        // Fill the queue with nodes that have in-degree of 0
        for (const auto& [procedure, degree]: inDegree) {
            if (degree == 0) {
                queue.push(procedure);
            }
        }

        std::vector<VL_SBX_PROCEDURE_TYPE> sortedProcedures;
        while (!queue.empty()) {
            const VL_SBX_PROCEDURE_TYPE current = queue.front();
            queue.pop();
            sortedProcedures.push_back(current);

            // Decrease the in-degree of neighboring nodes
            auto it = graph.find(current);
            if (it == graph.end()) {
                continue;
            }
            for (const VL_SBX_PROCEDURE_TYPE neighbor: it->second) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    queue.push(neighbor);
                }
            }
        }
        if (sortedProcedures.size() != inDegree.size()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Cycle detected in procedure dependencies. Topological sort failed.");
            return {};
        }
        return sortedProcedures;
    }

    void ProcedureExecutor::attachProcedures(const UP<Core::Context>& context, const UP<Core::Window>& window) const {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onAttach(context, window);
        }
    }
}
