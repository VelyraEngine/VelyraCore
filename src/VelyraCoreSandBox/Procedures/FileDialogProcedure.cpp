#include "FileDialogProcedure.hpp"

namespace Velyra::SandBox {

    void FileDialogProcedure::onImGui(DataContainer &container, const UP<Core::Context> &context, const UP<Core::Window>& window) {
        ImGui::Begin("Test File Dialogs");

        ImGui::InputText("Title", &title);
        ImGui::InputText("Default Path", &default_path);
        openFolderDialog(window);
        drawFilterOptions();
        ImGui::InputText("Filter Description", &filter_description);
        saveFileDialog(window);
        ImGui::Checkbox("Allow Multiple Selects", &allow_multiple_selects);
        openFileDialog(window);

        ImGui::End();
    }

    void FileDialogProcedure::openFolderDialog(const UP<Core::Window> &window) {
        if (ImGui::Button("Open Folder")) {
            Core::OpenFolderDesc desc;
            desc.title = title;
            desc.defaultPath = default_path;
            const auto result = window->openFolderDialog(desc);
            if (result.has_value()) {
                m_open_folder_result = result.value();
            }
        }
        if (m_open_folder_result.empty()) {
            ImGui::Text("No folder selected");
        }
        else {
            ImGui::Text("Selected Folder: %s", m_open_folder_result.string().c_str());
        }
    }

    void FileDialogProcedure::saveFileDialog(const UP<Core::Window> &window) {
        if (ImGui::Button("Save File")) {
            Core::SaveFileDesc desc;
            desc.title = title;
            desc.defaultPath = default_path;
            desc.filterPatterns = filter_patterns;
            desc.filterDescription = filter_description;
            const auto result = window->saveFileDialog(desc);
            if (result.has_value()) {
                m_save_file_result = result.value();
            }
        }
        if (m_save_file_result.empty()) {
            ImGui::Text("No file selected for saving");
        }
        else {
            ImGui::Text("File to Save: %s", m_save_file_result.string().c_str());
        }

    }

    void FileDialogProcedure::openFileDialog(const UP<Core::Window> &window) {
        if (ImGui::Button("Open File")) {
            Core::OpenFileDesc desc;
            desc.title = title;
            desc.defaultPath = default_path;
            desc.filterPatterns = filter_patterns;
            desc.filterDescription = filter_description;
            desc.allowMultipleSelects = allow_multiple_selects;
            const auto result = window->openFileDialog(desc);
            if (result.has_value()) {
                m_open_file_result = result.value();
            }
        }
        if (m_open_file_result.empty()) {
            ImGui::Text("No file selected for opening");
        }
        else {
            ImGui::Text("File to Open: %s", m_open_file_result.string().c_str());
        }

    }

    void FileDialogProcedure::drawFilterOptions() {
        ImGui::InputText("New Filter Pattern", &m_new_filter_pattern);
        ImGui::SameLine();
        if (ImGui::Button("Add Filter Pattern")) {
            if (!m_new_filter_pattern.empty()) {
                filter_patterns.push_back(m_new_filter_pattern);
                m_new_filter_pattern.clear();
            }
        }

        std::vector<size_t> patterns_to_remove;
        for (size_t i = 0; i < filter_patterns.size(); ++i) {
            ImGui::Text("%s", filter_patterns[i].c_str());
            ImGui::SameLine();
            std::string button_label = "Remove##" + std::to_string(i);
            if (ImGui::Button(button_label.c_str())) {
                patterns_to_remove.push_back(i);
            }
        }
        // Remove patterns in reverse order to avoid invalidating indices
        for (auto it = patterns_to_remove.rbegin(); it != patterns_to_remove.rend(); ++it) {
            filter_patterns.erase(filter_patterns.begin() + *it);
        }
    }
}
