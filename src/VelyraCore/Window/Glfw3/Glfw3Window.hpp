#pragma once

#include <VelyraCore/Window/Window.hpp>

#include <deque>
#include <mutex>

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    class Glfw3Window : public Window {
    public:
        explicit Glfw3Window(const WindowDesc& desc);

        ~Glfw3Window() override;

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

    private:
        static void setWindowStyleHints(VL_WINDOW_STYLE style);

        void createGlfwWindow(const WindowDesc &desc);

        void destroyGlfwWindow();

        void dispatchEvent(const Event& event);

        static void positionCallback(GLFWwindow* window, int xpos, int ypos);

        static void resizeCallback(GLFWwindow* window, int width, int height);

        static void closeCallback(GLFWwindow* window);

        static void refreshCallback(GLFWwindow* window);

        static void focusCallback(GLFWwindow* window, int focused);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void keyTypedCallback(GLFWwindow* window, unsigned int codepoint);

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    private:
        static Size m_GlfwWindowCount;

        std::deque<Event> m_EventQueue;
        std::mutex m_EventQueueMutex; // Since GLFW callbacks can be called from different threads

        Utils::LogPtr m_Logger = Utils::getLogger(VL_LOGGER_WINDOW);

        GLFWwindow* m_Window = nullptr;
        WindowDesc m_Desc; // Store it since we will be recreating the window if context switches happen
    };
}
