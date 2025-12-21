#include "ProcedureExecutor.hpp"

#include "Procedures/EventProcedure.hpp"
#include "Procedures/ImGuiProcedure.hpp"
#include "Procedures/ContextInfoProcedure.hpp"

namespace Velyra::SandBox {

    ProcedureExecutor::ProcedureExecutor() {
        registerProcedureFactory<EventProcedureFactory>();
        registerProcedureFactory<ImGuiProcedureFactory>();
        registerProcedureFactory<ContextInfoProcedureFactory>();
    }

    void ProcedureExecutor::addProcedure(const VL_SBX_PROCEDURE_TYPE procedureType) {
        m_RequiredProcedures.push_back(procedureType);
    }

    void ProcedureExecutor::constructStrategy() {
        for (const VL_SBX_PROCEDURE_TYPE procedure: m_RequiredProcedures) {
            auto itFactory = m_ProcedureFactories.find(procedure);
            if (itFactory == m_ProcedureFactories.end()) {
                SPDLOG_LOGGER_ERROR(m_Logger, "No factory found for procedure type {}", procedure);
                continue;
            }
            const auto& factory = itFactory->second;
            SPDLOG_LOGGER_INFO(m_Logger, "Adding procedure of type {}", procedure);
            // TODO: For now, don't handle dependencies between procedures
            m_ProceduresOrder.push_back(factory->createProcedure());
        }
    }

    void ProcedureExecutor::onUpdate(const Duration deltaTime) {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onUpdate(m_DataContainer, deltaTime);
        }
    }

    void ProcedureExecutor::onEvent(const Core::Event &event) {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onEvent(m_DataContainer, event);
        }
    }

    void ProcedureExecutor::onImGui(const UP<Core::Context> &context) {
        for (const auto& procedure: m_ProceduresOrder) {
            procedure->onImGui(m_DataContainer, context);
        }
    }
}