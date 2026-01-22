#include "../../Pch.hpp"

#include "Glfw3Window.hpp"
#include "Glfw3Utils.hpp"
#include "../../Context/OpenGL/Internal/Glfw3PlatformContext.hpp"
#include "../../Context/OpenGL/GLContext.hpp"

namespace Velyra::Core {

    Size Glfw3Window::m_GlfwWindowCount = 0;

    Glfw3Window::Glfw3Window(const WindowDesc &desc) {
        Glfw3Instance::createInstance();

        createGlfwWindow(desc);

        VL_POSTCONDITION(m_Window != nullptr, "Window creation failed!");
        VL_POSTCONDITION(glfwGetWindowUserPointer(m_Window) == this, "Failed to store pointer to client window!");
    }

    Glfw3Window::~Glfw3Window() {
        destroyGlfwWindow();
        Glfw3Instance::destroyInstance();
    }

    I32 Glfw3Window::getPositionX() const {
        int xPos;
        int yPos;
        glfwGetWindowPos(m_Window, &xPos, &yPos);
        return xPos;
    }

    I32 Glfw3Window::getPositionY() const {
        int xPos;
        int yPos;
        glfwGetWindowPos(m_Window, &xPos, &yPos);
        return yPos;
    }

    U32 Glfw3Window::getWidth() const {
        int width;
        int height;
        glfwGetWindowSize(m_Window, &width, &height);
        return static_cast<U32>(width);
    }

    U32 Glfw3Window::getHeight() const {
        int width;
        int height;
        glfwGetWindowSize(m_Window, &width, &height);
        return static_cast<U32>(height);
    }

    std::string Glfw3Window::getTitle() const {
        std::string title(glfwGetWindowTitle(m_Window));
        return title;
    }

    bool Glfw3Window::isOpen() const {
        return !glfwWindowShouldClose(m_Window);
    }

    bool Glfw3Window::isFullscreen() const {
        return false;
    }

    bool Glfw3Window::isFocused() const {
        return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED) == GLFW_TRUE;
    }

    bool Glfw3Window::hasEvent() const {
        // TODO: Should we lock the mutex here?
        return !m_EventQueue.empty();
    }

    void Glfw3Window::pollEvents() {
        glfwPollEvents();
    }

    Event Glfw3Window::getNextEvent() {
        std::lock_guard lock(m_EventQueueMutex);

        const Event event = m_EventQueue.front();
        m_EventQueue.pop_front();
        return event;
    }

    void Glfw3Window::close() {
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

    void Glfw3Window::setPosition(const I32 xPosition, const I32 yPosition) {
        glfwSetWindowPos(m_Window, xPosition, yPosition);
    }

    void Glfw3Window::setSize(const U32 width, const U32 height) {
        glfwSetWindowSize(m_Window, static_cast<int>(width), static_cast<int>(height));
    }

    void Glfw3Window::setTitle(const std::string &title) {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void Glfw3Window::show() {
        glfwShowWindow(m_Window);
    }

    void Glfw3Window::hide() {
        glfwHideWindow(m_Window);
    }

    void Glfw3Window::enableFullscreen() {

    }

    void Glfw3Window::disableFullscreen() {

    }

    void Glfw3Window::showMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Glfw3Window::hideMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Glfw3Window::grabMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    }

    void Glfw3Window::releaseMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Glfw3Window::setMousePosition(const I32 mousePosX, const I32 mousePosY) {
        // Note: GLFW does not provide a direct way to set the mouse position relative to the window.
        // We would need to get the window position on the screen and add the offsets.
        int windowX = 0;
        int windowY = 0;
        glfwGetWindowPos(m_Window, &windowX, &windowY);
        glfwSetCursorPos(m_Window, mousePosX, mousePosY);
    }

    I32 Glfw3Window::getMousePositionX() const {
        return 0;
    }

    I32 Glfw3Window::getMousePositionY() const {
        return 0;
    }

    float Glfw3Window::getDpiScale() const {
        float xScale;
        float yScale;
        glfwGetWindowContentScale(m_Window, &xScale, &yScale);
        if (xScale - yScale > 0.01f) {
            SPDLOG_LOGGER_WARN(m_Logger, "Non-uniform DPI scaling detected: xScale = {}, yScale = {}", xScale, yScale);
        }
        return xScale; // Assume x and y are the same
    }

    std::optional<fs::path> Glfw3Window::saveFileDialog(const SaveFileDesc &desc) {
        // Convert filter patterns to the format expected by tinyfiledialogs
        std::vector<const char*> raw_filter_patterns;
        raw_filter_patterns.reserve(desc.filterPatterns.size());
        for (const auto& pattern : desc.filterPatterns) {
            raw_filter_patterns.push_back(pattern.data());
        }

        char const* aTitle = desc.title.empty()? "Open File" : desc.title.c_str();
        char const* aDefaultPathAndFile = desc.defaultPath.empty()? "" : desc.defaultPath.c_str();
        const int aNumOfFilterPatterns = static_cast<int>(raw_filter_patterns.size());
        char const* const* const aFilterPatterns = aNumOfFilterPatterns > 0 ? raw_filter_patterns.data() : nullptr;
        char const* aSingleFilterDescription = desc.filterDescription.empty() ? nullptr : desc.filterDescription.c_str();

        char const* result = tinyfd_saveFileDialog(
            aTitle,
            aDefaultPathAndFile,
            aNumOfFilterPatterns,
            aFilterPatterns,
            aSingleFilterDescription
        );
        if (result) {
            return fs::path(result);
        }
        return std::nullopt;
    }

    std::optional<fs::path> Glfw3Window::openFileDialog(const OpenFileDesc &desc) {
        // Convert filter patterns to the format expected by tinyfiledialogs
        std::vector<const char*> raw_filter_patterns;
        raw_filter_patterns.reserve(desc.filterPatterns.size());
        for (const auto& pattern : desc.filterPatterns) {
            raw_filter_patterns.push_back(pattern.data());
        }

        char const* aTitle = desc.title.empty()? "Open File" : desc.title.c_str();
        char const* aDefaultPathAndFile = desc.defaultPath.empty()? "" : desc.defaultPath.c_str();
        const int aNumOfFilterPatterns = static_cast<int>(raw_filter_patterns.size());
        char const* const* const aFilterPatterns = aNumOfFilterPatterns > 0 ? raw_filter_patterns.data() : nullptr;
        char const* aSingleFilterDescription = desc.filterDescription.empty() ? nullptr : desc.filterDescription.c_str();
        const int aAllowMultipleSelects = desc.allowMultipleSelects;

        char const* result = tinyfd_openFileDialog(
            aTitle,
            aDefaultPathAndFile,
            aNumOfFilterPatterns,
            aFilterPatterns,
            aSingleFilterDescription,
            aAllowMultipleSelects
        );
        if (result) {
            return fs::path(result);
        }
        return std::nullopt;
    }

    std::optional<fs::path> Glfw3Window::openFolderDialog(const OpenFolderDesc &desc) {
        char const* aTitle = desc.title.empty()? "Open Folder" : desc.title.c_str();
        char const* aDefaultPath = desc.defaultPath.empty()? "" : desc.defaultPath.c_str();
        char const* result = tinyfd_selectFolderDialog(
            aTitle,
            aDefaultPath
        );
        if (result) {
            return fs::path(result);
        }
        return std::nullopt;
    }

    const UP<Context> &Glfw3Window::createContext(const ContextDesc &desc) {
        destroyGlfwWindow(); // Destroy the old window if it exists

        Glfw3PlatformContext::setWindowHints(desc);
        createGlfwWindow(m_Desc);

        const VL_GRAPHICS_API api = desc.api;
        switch (api) {
            case VL_API_BEST:
            case VL_API_OPENGL: {
                UP<GLPlatformContext> platformContext = createUP<Glfw3PlatformContext>(desc, m_Window);
                m_Context = createUP<GLContext>(desc, std::move(platformContext));
                break;
            }
            default: {
                SPDLOG_LOGGER_ERROR(m_Logger, "Unsupported graphics API {} for Glfw3Window", api);
            }
        }
        return m_Context;
    }

    //////////////////// PRIVATE METHODS ////////////////////

    void Glfw3Window::setWindowStyleHints(const VL_WINDOW_STYLE style) {
        if (style == VL_WINDOW_STYLE_DEFAULT) {
            return; // don't set anything since this defaults to GLFW's default style
        }
        if (style == VL_WINDOW_STYLE_POPUP) {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            return;
        }

        if (!(style & VL_WINDOW_STYLE_RESIZE)) {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }
    }

    void Glfw3Window::createGlfwWindow(const WindowDesc &desc) {
        VL_PRECONDITION(m_Window == nullptr, "GLFWwindow already created!");

        m_Desc = desc; // Store it for potential recreation
        setWindowStyleHints(m_Desc.style);
        m_Window = glfwCreateWindow(
            static_cast<int>(m_Desc.width),
            static_cast<int>(m_Desc.height),
            m_Desc.title.c_str(),
            nullptr,
            nullptr
        );
        if (!m_Window) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to create GLFW window");
            return;
        }
        glfwShowWindow(m_Window);
        glfwSetWindowUserPointer(m_Window, this);

        // Send VL_EVENT_WINDOW_OPENED event
        const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_OPENED);
        dispatchEvent(event);
        
        // Then, set all window callbacks
        glfwSetWindowPosCallback(m_Window, positionCallback);
        glfwSetWindowSizeCallback(m_Window, resizeCallback);
        glfwSetWindowCloseCallback(m_Window, closeCallback);
        glfwSetWindowRefreshCallback(m_Window, refreshCallback);
        glfwSetWindowFocusCallback(m_Window, focusCallback);

        // Next, all input callbacks
        glfwSetKeyCallback(m_Window, keyCallback);
        glfwSetCharCallback(m_Window, keyTypedCallback);
        glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, mousePositionCallback);
        glfwSetScrollCallback(m_Window, mouseScrollCallback);

        VL_POSTCONDITION(m_Window != nullptr, "Failed to create GLFW window!");
    }

    void Glfw3Window::destroyGlfwWindow() {
        if (!m_Window) {
            return;
        }
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    void Glfw3Window::dispatchEvent(const Event &event) {
        std::lock_guard lock(m_EventQueueMutex);
        m_EventQueue.push_back(event);
    }
    
    void Glfw3Window::positionCallback(GLFWwindow *window, const int xpos, const int ypos) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_MOVED, xpos, ypos);
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::resizeCallback(GLFWwindow *window, const int width, const int height) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_RESIZED, width, height);
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::closeCallback(GLFWwindow *window) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_CLOSED);
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::refreshCallback(GLFWwindow *window) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_REFRESHED);
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::focusCallback(GLFWwindow *window, const int focused) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        if (focused == GLFW_TRUE) {
            const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_GAINED_FOCUS);
            glfw3Window->dispatchEvent(event);
        }
        else {
            const Event event(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_LOST_FOCUS);
            glfw3Window->dispatchEvent(event);
        }
    }

    void Glfw3Window::keyCallback(GLFWwindow *window, const int key, const int /*scancode*/, const int action, const int /*mods*/) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const VL_KEYBOARD_KEY translatedKey = translateGlfwKey(key);
        switch (action) {
            case GLFW_PRESS: {
                const Event event(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_PRESSED, translatedKey);
                glfw3Window->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE: {
                const Event event(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_RELEASED, translatedKey);
                glfw3Window->dispatchEvent(event);
                break;
            }
            default: break;
        }
    }

    void Glfw3Window::keyTypedCallback(GLFWwindow *window, const unsigned int codepoint) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_TYPED, 0, 0, static_cast<char>(codepoint));
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::mouseButtonCallback(GLFWwindow *window, const int button, const int action, const int /*mods*/) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const VL_MOUSE_BUTTON mouseButton = translateGlfwMouseButton(button);
        switch (action) {
            case GLFW_PRESS: {
                const Event event(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_PRESSED, mouseButton);
                glfw3Window->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE: {
                const Event event(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_RELEASED, mouseButton);
                glfw3Window->dispatchEvent(event);
                break;
            }
            default: break;
        }
    }

    void Glfw3Window::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_MOVED, static_cast<I32>(xpos), static_cast<I32>(ypos));
        glfw3Window->dispatchEvent(event);
    }

    void Glfw3Window::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        const auto glfw3Window = static_cast<Glfw3Window*>(glfwGetWindowUserPointer(window));

        const Event event(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_SCROLLED, static_cast<I32>(xoffset), static_cast<I32>(yoffset));
        glfw3Window->dispatchEvent(event);
    }
}
