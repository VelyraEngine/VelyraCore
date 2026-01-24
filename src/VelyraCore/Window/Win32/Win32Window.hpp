#pragma once

#include <VelyraCore/Window/Window.hpp>
#include "../../Logging/LoggerNames.hpp"
#include "../../Logging/Win32Logging.hpp"

#include <deque>

namespace Velyra::Core {

    struct FullScreenInfo {
        bool maximized = false;
        LONG_PTR style = 0;
        LONG_PTR exStyle = 0;
        RECT rect = {0, 0, 0, 0};
    };

    class Win32Window : public Window {
    public:
        Win32Window(const WindowDesc& desc);

        ~Win32Window() override;

        I32 getPositionX() const override;

        I32 getPositionY() const override;

        U32 getWidth() const override;

        U32 getHeight() const override;

        std::string getTitle() const override;

        bool isOpen() const override;

        bool isFullscreen() const override;

        bool isFocused() const override;

        bool hasEvent() const override;

        void pollEvents() override;

        Event getNextEvent() override;

        void close() override;

        void setPosition(I32 xPosition, I32 yPosition) override;

        void setSize(U32 width, U32 height) override;

        void setTitle(const std::string& title) override;

        void show() override;

        void hide() override;

        void enableFullscreen() override;

        void disableFullscreen() override;

        void showMouse() override;

        void hideMouse() override;

        void grabMouse() override;

        void releaseMouse() override;

        void setMousePosition(I32 mousePosX, I32 mousePosY) override;

        I32 getMousePositionX() const override;

        I32 getMousePositionY() const override;

        float getDpiScale() const override;

        std::optional<fs::path> saveFileDialog(const SaveFileDesc& desc) override;

        std::vector<fs::path> openFileDialog(const OpenFileDesc& desc) override;

        std::optional<fs::path> openFolderDialog(const OpenFolderDesc& desc) override;

        const UP<Context>& createContext(const ContextDesc &desc) override;

        static LRESULT CALLBACK windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    private:

        void registerRawInputDevices() const;

        void handleEvent(UINT msg, WPARAM wparam, LPARAM lparam);

        void mouseTracker(bool enableTracking) const;

        /**
         * @brief Creates a file dialog of the specified type.
         * @tparam DIALOG_TYPE The type of dialog to create (IFileOpenDialog, IFileSaveDialog, etc).
         * @return Pointer to the created dialog, or nullptr if creation failed.
         */
        template<typename DIALOG_TYPE>
        DIALOG_TYPE* createFileDialog() const {
            CLSID clsid;

            if constexpr(std::is_same_v<DIALOG_TYPE, IFileOpenDialog>) {
                clsid = CLSID_FileOpenDialog;
            }
            else if constexpr(std::is_same_v<DIALOG_TYPE, IFileSaveDialog>) {
                clsid = CLSID_FileSaveDialog;
            }
            else {
                SPDLOG_LOGGER_ERROR(m_Logger, "Unsupported dialog type for Win32Window");
                return nullptr;
            }

            DIALOG_TYPE* pDialog = nullptr;
            HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));
            if (!decodeHRESULT(hr)) {
                return nullptr;
            }
            return pDialog;
        }

        void setDialogTitle(IFileDialog* pDialog, const std::string& title) const;

        void setDialogDefaultPath(IFileDialog* pDialog, const fs::path& defaultPath) const;

        void setDialogFilters(IFileDialog* pDialog, const std::vector<std::string>& filterPatterns, const std::string& filterDescription) const;

        std::wstring buildPatternList(const std::vector<std::string>& filterPatterns) const;

        void setDialogOptions(IFileDialog* pDialog, DWORD options) const;

        std::vector<fs::path> getDialogResults(IFileDialog* pDialog) const;

    private:
        static Size m_WindowCount;

        Utils::LogPtr m_Logger = Utils::getLogger(VL_LOGGER_WINDOW);

        HWND m_HWND = nullptr;
        HINSTANCE m_HInstance = nullptr;

        std::deque<Event> m_EventQueue;

        FullScreenInfo m_FullScreenInfo;

        bool m_Open = false;
        bool m_Focused = false;
        bool m_Fullscreen = false;
        bool m_MouseGrabbed = false;
        bool m_Moving = false;
        bool m_Resizing = false;
        bool m_MouseInside = false;

    };

}