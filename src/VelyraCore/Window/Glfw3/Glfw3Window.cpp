#include "../../Pch.hpp"

#include "../../Context/OpenGL/Internal/Glfw3PlatformContext.hpp"
#include "../../Context/OpenGL/GLContext.hpp"
#include "../../Context/OpenGLES/Internal/Glfw3GLESPlatformContext.hpp"
#include "../../Context/OpenGLES/GLESContext.hpp"
#include "Glfw3Utils.hpp"
#include "Glfw3Window.hpp"
#include "VelyraUtils/Logging/Logging.hpp"

#define GLFW_EXPOSE_NATIVE_X11
#include <nfd_glfw3.h>

namespace Velyra::Core {

    namespace {
        void buildDialogFilters(const std::vector<std::string>& patterns,
                                const std::string& description,
                                std::string& filterName,
                                std::string& filterSpec,
                                std::vector<nfdu8filteritem_t>& filters) {
            filterName = description.empty() ? "Files" : description;

            for (const std::string& pattern : patterns) {
                std::string normalizedPattern = pattern;
                std::replace(normalizedPattern.begin(), normalizedPattern.end(), ';', ',');
                std::stringstream patternStream(normalizedPattern);
                std::string extension;
                while (std::getline(patternStream, extension, ',')) {
                    if (extension.starts_with("*.")) {
                        extension.erase(0, 2);
                    }
                    else if (extension.starts_with('.')) {
                        extension.erase(0, 1);
                    }

                    if (extension.empty() || extension == "*" ||
                        extension.find_first_of("*?/\\") != std::string::npos) {
                        continue;
                    }

                    if (!filterSpec.empty()) {
                        filterSpec += ',';
                    }
                    filterSpec += extension;
                }
            }

            if (!filterSpec.empty()) {
                filters.push_back({filterName.c_str(), filterSpec.c_str()});
            }
        }

        nfdwindowhandle_t getNativeWindowHandle(GLFWwindow* window) {
            nfdwindowhandle_t nativeWindow{};
            NFD_GetNativeWindowFromGLFWWindow(window, &nativeWindow);
            return nativeWindow;
        }

        void setDisplayProperties(const Utils::LogPtr& logger) {
            if (!NFD_SetDisplayPropertiesFromGLFW()) {
                const char* error = NFD::GetError();
                SPDLOG_LOGGER_WARN(logger, "Failed to configure native file dialog display: {}", error ? error : "unknown error");
            }
        }

        void logDialogError(const Utils::LogPtr& logger, const char* operation) {
            const char* error = NFD::GetError();
            SPDLOG_LOGGER_WARN(logger, "{} file dialog failed: {}", operation, error ? error : "unknown error");
        }
    }

    Size Glfw3Window::m_GlfwWindowCount = 0;

    Glfw3Window::Glfw3Window(const WindowDesc &desc) {
        Glfw3Instance::createInstance();

        createGlfwWindow(desc);

        VL_POSTCONDITION(m_Window != nullptr, "Window creation failed!");
        VL_POSTCONDITION(glfwGetWindowUserPointer(m_Window) == this, "Failed to store pointer to client window!");
    }

    Glfw3Window::~Glfw3Window() {
        // Destroy the context before destroying the window
        if (m_Context) {
            m_Context.reset();
        }
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

    void Glfw3Window::setIcon(const Image::IImage &image) {
        // Checks to see if any conversions should be done
        if (image.getDataType() == VL_UINT8 && image.getChannelFormat() == VL_CHANNEL_RGBA) {
            setIconInternal(image);
            return;
        }

        if (image.getChannelFormat() != VL_CHANNEL_RGBA) {
            SPDLOG_LOGGER_PERFORMANCE(m_Logger, "Window Icon is in format {}, format VL_CHANNEL_RGBA is required so the image will be converted. To increase performance, consider doing this conversion beforehand", image.getChannelFormat());

            Image::FormatConversionDesc formatConversionDesc;
            formatConversionDesc.fillMode = VL_FILL_MIN;
            formatConversionDesc.targetFormat = VL_CHANNEL_RGBA;
            UP<Image::IImage> convertedImage = image.convertToFormat(formatConversionDesc);

            if (convertedImage->getDataType() != VL_UINT8) {
                SPDLOG_LOGGER_PERFORMANCE(m_Logger, "Window Icon is in data type {}, data type VL_UINT8 is required so the image will be converted. To increase performance, consider doing this conversion beforehand", convertedImage->getDataType());

                Image::TranslationDesc translationDesc;
                translationDesc.targetType = VL_UINT8;
                convertedImage = convertedImage->translateDataType(translationDesc);
            }
            setIconInternal(*convertedImage);
            return;
        }

        if (image.getDataType() != VL_UINT8) {
            SPDLOG_LOGGER_PERFORMANCE(m_Logger, "Window Icon is in data type {}, data type VL_UINT8 is required so the image will be converted. To increase performance, consider doing this conversion beforehand", image.getDataType());

            Image::TranslationDesc translationDesc;
            translationDesc.targetType = VL_UINT8;
            const UP<Image::IImage> convertedImage = image.translateDataType(translationDesc);
            setIconInternal(*convertedImage);
        }
    }

    std::optional<fs::path> Glfw3Window::saveFileDialog(const SaveFileDesc &desc) {
        if (!Glfw3Instance::isFileDialogInitialized()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Native file dialogs are not initialized");
            return std::nullopt;
        }

        setDisplayProperties(m_Logger);
        std::string filterName;
        std::string filterSpec;
        std::vector<nfdu8filteritem_t> filters;
        buildDialogFilters(desc.filterPatterns, desc.filterDescription, filterName, filterSpec, filters);

        nfdsavedialogu8args_t args{};
        args.filterList = filters.empty() ? nullptr : filters.data();
        args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
        args.defaultPath = desc.defaultPath.empty() ? nullptr : desc.defaultPath.c_str();
        args.parentWindow = getNativeWindowHandle(m_Window);
        args.title = desc.title.empty() ? nullptr : desc.title.c_str();

        NFD::UniquePathU8 result;
        nfdu8char_t* rawPath = nullptr;
        const nfdresult_t dialogResult = NFD_SaveDialogU8_With(&rawPath, &args);
        if (dialogResult == NFD_OKAY) {
            result.reset(rawPath);
            return fs::path(result.get());
        }
        if (dialogResult == NFD_ERROR) {
            logDialogError(m_Logger, "Save");
        }
        return std::nullopt;
    }

    std::vector<fs::path> Glfw3Window::openFileDialog(const OpenFileDesc &desc) {
        std::vector<fs::path> paths;
        if (!Glfw3Instance::isFileDialogInitialized()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Native file dialogs are not initialized");
            return paths;
        }

        setDisplayProperties(m_Logger);
        std::string filterName;
        std::string filterSpec;
        std::vector<nfdu8filteritem_t> filters;
        buildDialogFilters(desc.filterPatterns, desc.filterDescription, filterName, filterSpec, filters);

        nfdopendialogu8args_t args{};
        args.filterList = filters.empty() ? nullptr : filters.data();
        args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
        args.defaultPath = desc.defaultPath.empty() ? nullptr : desc.defaultPath.c_str();
        args.parentWindow = getNativeWindowHandle(m_Window);
        args.title = desc.title.empty() ? nullptr : desc.title.c_str();

        if (desc.allowMultipleSelects) {
            NFD::UniquePathSet pathSet;
            const nfdpathset_t* rawPathSet = nullptr;
            const nfdresult_t dialogResult = NFD_OpenDialogMultipleU8_With(&rawPathSet, &args);
            if (dialogResult == NFD_ERROR) {
                logDialogError(m_Logger, "Open");
                return paths;
            }
            if (dialogResult == NFD_CANCEL) {
                return paths;
            }

            pathSet.reset(rawPathSet);
            nfdpathsetsize_t pathCount = 0;
            if (NFD::PathSet::Count(pathSet.get(), pathCount) == NFD_OKAY) {
                for (nfdpathsetsize_t index = 0; index < pathCount; ++index) {
                    NFD::UniquePathSetPath path;
                    if (NFD::PathSet::GetPath(pathSet, index, path) == NFD_OKAY) {
                        paths.emplace_back(path.get());
                    }
                }
            }
            return paths;
        }

        NFD::UniquePathU8 path;
        nfdu8char_t* rawPath = nullptr;
        const nfdresult_t dialogResult = NFD_OpenDialogU8_With(&rawPath, &args);
        if (dialogResult == NFD_OKAY) {
            path.reset(rawPath);
            paths.emplace_back(path.get());
        }
        else if (dialogResult == NFD_ERROR) {
            logDialogError(m_Logger, "Open");
        }
        return paths;
    }

    std::optional<fs::path> Glfw3Window::openFolderDialog(const OpenFolderDesc &desc) {
        if (!Glfw3Instance::isFileDialogInitialized()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Native file dialogs are not initialized");
            return std::nullopt;
        }

        setDisplayProperties(m_Logger);
        nfdpickfolderu8args_t args{};
        args.defaultPath = desc.defaultPath.empty() ? nullptr : desc.defaultPath.c_str();
        args.parentWindow = getNativeWindowHandle(m_Window);
        args.title = desc.title.empty() ? nullptr : desc.title.c_str();

        NFD::UniquePathU8 result;
        nfdu8char_t* rawPath = nullptr;
        const nfdresult_t dialogResult = NFD_PickFolderU8_With(&rawPath, &args);
        if (dialogResult == NFD_OKAY) {
            result.reset(rawPath);
            return fs::path(result.get());
        }
        if (dialogResult == NFD_ERROR) {
            logDialogError(m_Logger, "Folder");
        }
        return std::nullopt;
    }

    const UP<Context> &Glfw3Window::createContext(const ContextDesc &desc) {
        destroyGlfwWindow(); // Destroy the old window if it exists

        const VL_GRAPHICS_API api = desc.api;
        switch (api) {
            case VL_API_BEST:
            case VL_API_OPENGL: {
                Glfw3PlatformContext::setWindowHints(desc);
                createGlfwWindow(m_Desc);

                UP<GLPlatformContext> platformContext = createUP<Glfw3PlatformContext>(desc, m_Window);
                m_Context = createUP<GLContext>(desc, std::move(platformContext));
                break;
            }
            case VL_API_OPENGL_ES: {
                GLES::Glfw3GLESPlatformContext::setWindowHints(desc);
                createGlfwWindow(m_Desc);

                UP<GLES::GLESPlatformContext> platformContext = createUP<GLES::Glfw3GLESPlatformContext>(desc, m_Window);
                m_Context = createUP<GLES::GLESContext>(desc, std::move(platformContext));
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

    void Glfw3Window::setIconInternal(const Image::IImage &image) const {
        VL_PRECONDITION(image.getDataType() == VL_UINT8, "Image data type must be VL_UINT8");
        VL_PRECONDITION(image.getChannelFormat() == VL_CHANNEL_RGBA, "Image channel format must be VL_CHANNEL_RGBA")
        VL_PRECONDITION(m_Window != nullptr, "GLFWwindow is null");

        std::vector<UP<Image::IImage>> icons;
        std::vector<GLFWimage> glfwIcons;

        static constexpr Size iconSizes[] = {16, 32, 48, 64, 128};

        for (const Size size: iconSizes) {
            icons.emplace_back(image.resize(size, size));
            glfwIcons.push_back({
                static_cast<int>(size),
                static_cast<int>(size),
                static_cast<unsigned char*>(icons.back()->getData())
            });
        }

        glfwSetWindowIcon(m_Window, static_cast<int>(glfwIcons.size()), glfwIcons.data());
    }
}
