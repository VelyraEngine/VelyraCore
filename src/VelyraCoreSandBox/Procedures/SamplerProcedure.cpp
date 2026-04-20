#include "SamplerProcedure.hpp"

namespace Velyra::SandBox {

    void SamplerProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        Core::SamplerDesc splrDesc;
        m_Sampler = context->createSampler(splrDesc);

        // Ensure that the string representations of the texture filter and wrap modes are generated before we use them
        // in ImGui
        if (!VL_TEXTURE_FILTER_detail::VL_TEXTURE_FILTER_StructuresGenerated) {
            VL_TEXTURE_FILTER_detail::generateStructures();
        }
        if (!VL_TEXTURE_WRAP_detail::VL_TEXTURE_WRAP_StructuresGenerated) {
            VL_TEXTURE_WRAP_detail::generateStructures();
        }
        if (!VL_COMPARISON_FUNC_detail::VL_COMPARISON_FUNC_StructuresGenerated) {
            VL_COMPARISON_FUNC_detail::generateStructures();
        }
    }

    void SamplerProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_Sampler->bindShaderResource(0);
    }

    void SamplerProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Sampler");
        ImGui::Text("Min Filter Mode: %s", Utils::toString(m_Sampler->getMinFilter()).c_str());
        ImGui::Text("Mag Filter Mode: %s", Utils::toString(m_Sampler->getMagFilter()).c_str());
        ImGui::Text("Wrap U Mode: %s", Utils::toString(m_Sampler->getWrapU()).c_str());
        ImGui::Text("Wrap V Mode: %s", Utils::toString(m_Sampler->getWrapV()).c_str());
        ImGui::Text("Wrap W Mode: %s", Utils::toString(m_Sampler->getWrapW()).c_str());

        if (ImGui::BeginCombo("Min Filter", Utils::toString(m_Sampler->getMinFilter()).c_str())) {
            for (int i = 0; i < 2; i++) {
                auto filter = static_cast<VL_TEXTURE_FILTER>(i + 1);
                const bool isSelected = (m_Sampler->getMinFilter() == filter);
                if (ImGui::Selectable(Utils::toString(filter).c_str(), isSelected)) {
                    m_Sampler->setFilter(filter, m_Sampler->getMagFilter());
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Mag Filter", Utils::toString(m_Sampler->getMagFilter()).c_str())) {
            for (int i = 0; i < 2; i++) {
                auto filter = static_cast<VL_TEXTURE_FILTER>(i + 1);
                const bool isSelected = (m_Sampler->getMagFilter() == filter);
                if (ImGui::Selectable(Utils::toString(filter).c_str(), isSelected)) {
                    m_Sampler->setFilter(m_Sampler->getMinFilter(), filter);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Wrap U", Utils::toString(m_Sampler->getWrapU()).c_str())) {
            for (int i = 0; i < 4; i++) {
                auto wrapMode = static_cast<VL_TEXTURE_WRAP>(i + 1);
                const bool isSelected = (m_Sampler->getWrapU() == wrapMode);
                if (ImGui::Selectable(Utils::toString(wrapMode).c_str(), isSelected)) {
                    m_Sampler->setWrap(wrapMode, m_Sampler->getWrapV(), m_Sampler->getWrapW());
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Wrap V", Utils::toString(m_Sampler->getWrapV()).c_str())) {
            for (int i = 0; i < 4; i++) {
                auto wrapMode = static_cast<VL_TEXTURE_WRAP>(i + 1);
                const bool isSelected = (m_Sampler->getWrapV() == wrapMode);
                if (ImGui::Selectable(Utils::toString(wrapMode).c_str(), isSelected)) {
                    m_Sampler->setWrap(m_Sampler->getWrapU(), wrapMode, m_Sampler->getWrapW());
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Wrap W", Utils::toString(m_Sampler->getWrapW()).c_str())) {
            for (int i = 0; i < 4; i++) {
                auto wrapMode = static_cast<VL_TEXTURE_WRAP>(i + 1);
                const bool isSelected = (m_Sampler->getWrapW() == wrapMode);
                if (ImGui::Selectable(Utils::toString(wrapMode).c_str(), isSelected)) {
                    m_Sampler->setWrap(m_Sampler->getWrapU(), m_Sampler->getWrapV(), wrapMode);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }

        ImGui::End();

    }
}
