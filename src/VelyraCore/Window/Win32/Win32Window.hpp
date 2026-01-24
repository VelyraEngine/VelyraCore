#pragma once

#include <VelyraCore/Window/Window.hpp>
#include "../../Logging/LoggerNames.hpp"

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

        std::optional<fs::path> openFileDialog(const OpenFileDesc& desc) override;

        std::optional<fs::path> openFolderDialog(const OpenFolderDesc& desc) override;

        const UP<Context>& createContext(const ContextDesc &desc) override;

    private:

        void registerWindowClass() const;

        void unregisterWindowClass() const;

        void registerRawInputDevices() const;

        static LRESULT CALLBACK windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        void handleEvent(UINT msg, WPARAM wparam, LPARAM lparam);

        void mouseTracker(bool enableTracking) const;

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