#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    class FileDialogProcedure: public IProcedure {
    public:
        FileDialogProcedure() = default;

        ~FileDialogProcedure() override = default;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window>& window) override;

    private:

        void openFolderDialog(const UP<Core::Window>& window);

        void saveFileDialog(const UP<Core::Window>& window);

        void openFileDialog(const UP<Core::Window>& window);

        void drawFilterOptions();

    private:
        std::string title;
        std::string default_path;
        std::vector<std::string> filter_patterns;
        std::string filter_description;
        bool allow_multiple_selects = false;

        // Outputs
        std::filesystem::path m_open_folder_result;
        std::filesystem::path m_save_file_result;
        std::filesystem::path m_open_file_result;

        // Internal state
        std::string m_new_filter_pattern;
    };

    VL_DECLARE_PROCEDURE_FACTORY(FileDialogProcedure, FileDialog, {})

}