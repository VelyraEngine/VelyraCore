#include "../../Pch.hpp"

#include "WindowWin32.hpp"
#include "Win32Utils.hpp"

#include "../../Context/OpenGL/Internal/WglPlatformContext.hpp"
#include "../../Context/OpenGL/GLContext.hpp"

namespace Velyra::Core {

    constexpr auto VL_CORE_WIN32_CLASS_NAME = L"VelyraWindowClass";

    Size WindowWin32::m_WindowCount = 0;

    WindowWin32::WindowWin32(const WindowDesc &desc):
    Window(),
    m_HInstance(GetModuleHandleW(nullptr)){
        if (m_WindowCount == 0) {
            registerWindowClass();
            setProcessDPIAware();
        }
        m_WindowCount++;

        m_FullScreenInfo.maximized = false;
        m_FullScreenInfo.style = decodeWindowStyle(desc.style);
        m_FullScreenInfo.exStyle = decodeExtendedWindowStyle(desc.style);
        m_FullScreenInfo.rect = {0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height)};

        const std::wstring wideTitle = toWideString(desc.title);
        m_HWND = CreateWindowExW(m_FullScreenInfo.exStyle,
                                 VL_CORE_WIN32_CLASS_NAME,
                                 wideTitle.c_str(),
                                 m_FullScreenInfo.style,
                                 desc.xPosition, desc.yPosition,
                                 desc.width, desc.height,
                                 nullptr, nullptr,
                                 m_HInstance,
                                 this);
        if (!m_HWND) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to create Win32 window");
            return;
        }
        // store a pointer to our Window on the Win32 API side
        SetWindowLongPtrW(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        ShowWindow(m_HWND, SW_SHOW);
        SetFocus(m_HWND);
        m_Open = true;
        m_Focused = true;

        registerRawInputDevices();

        SPDLOG_LOGGER_INFO(m_Logger, "Created Win32 window '{}' with size ({}x{}) at position ({}, {})", desc.title, desc.width, desc.height, desc.xPosition, desc.yPosition);

        VL_POSTCONDITION(m_HWND != nullptr, "WindowWin32", "Window handle is null after creation");
    }

    WindowWin32::~WindowWin32() {
        if (m_HWND) {
            DestroyWindow(m_HWND);
        }
        m_WindowCount--;
        if (m_WindowCount == 0) {
            unregisterWindowClass();
        }
    }

    I32 WindowWin32::getPositionX() const {
        RECT windowRect;
        GetWindowRect(m_HWND, &windowRect);
        return windowRect.left;
    }

    I32 WindowWin32::getPositionY() const {
        RECT windowRect;
        GetWindowRect(m_HWND, &windowRect);
        return windowRect.top;
    }

    U32 WindowWin32::getWidth() const {
        RECT windowRect;
        GetWindowRect(m_HWND, &windowRect);
        return static_cast<U32>(windowRect.right - windowRect.left);
    }

    U32 WindowWin32::getHeight() const {
        RECT windowRect;
        GetWindowRect(m_HWND, &windowRect);
        return static_cast<U32>(windowRect.bottom - windowRect.top);
    }

    std::string WindowWin32::getTitle() const {
        const int length = GetWindowTextLengthW(m_HWND);

        std::wstring wideTitle(length, L'\0');
        GetWindowTextW(m_HWND, wideTitle.data(), length + 1);

        wideTitle.resize(length); // REMOVE null terminator, we add it ourselves in fromWideString

        return fromWideString(wideTitle);
    }

    bool WindowWin32::isOpen() const {
        return m_Open;
    }

    bool WindowWin32::isFullscreen() const {
        return m_Fullscreen;
    }

    bool WindowWin32::isFocused() const {
        return m_Focused;
    }

    bool WindowWin32::hasEvent() const {
        return !m_EventQueue.empty();
    }

    void WindowWin32::pollEvents() {
        MSG msg;
        while (PeekMessageW(&msg, m_HWND, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    Event WindowWin32::getNextEvent() {
        const Event event = m_EventQueue.front();
        m_EventQueue.pop_front();
        return event;
    }

    void WindowWin32::close() {
        m_Open = false;
        ShowWindow(m_HWND, SW_HIDE);
    }

    void WindowWin32::setPosition(const I32 xPosition, const I32 yPosition) {
        if (m_Fullscreen) {
            return;
        }
        SetWindowPos(m_HWND, nullptr, xPosition, yPosition, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    void WindowWin32::setSize(const U32 width, const U32 height) {
        if (m_Fullscreen) {
            return;
        }
        SetWindowPos(m_HWND, nullptr, 0, 0, static_cast<I32>(width), static_cast<I32>(height), SWP_NOMOVE | SWP_NOZORDER);
    }

    void WindowWin32::setTitle(const std::string &title) {
        const std::wstring wideTitle = toWideString(title);
        SetWindowTextW(m_HWND, wideTitle.c_str());
    }

    void WindowWin32::show() {
        if (!ShowWindow(m_HWND, SW_SHOW)) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to show Win32 window");
        }
    }

    void WindowWin32::hide() {
        if (!ShowWindow(m_HWND, SW_HIDE)) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to hide Win32 window");
        }
    }

    void WindowWin32::enableFullscreen() {
        if (m_Fullscreen) {
            return;
        }
        m_FullScreenInfo.maximized = IsZoomed(m_HWND) != 0;
        m_FullScreenInfo.style = GetWindowLongPtrW(m_HWND, GWL_STYLE);
        m_FullScreenInfo.exStyle = GetWindowLongPtrW(m_HWND, GWL_EXSTYLE);
        GetWindowRect(m_HWND, &m_FullScreenInfo.rect);

        // Get the dimensions of the primary monitor
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO);
        HMONITOR monitor = MonitorFromWindow(m_HWND, MONITOR_DEFAULTTONEAREST);
        if (!GetMonitorInfoW(monitor, &monitorInfo)) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to get monitor info for fullscreen");
            return;
        }
        SetWindowLongW(m_HWND, GWL_STYLE, m_FullScreenInfo.style & ~(WS_CAPTION | WS_THICKFRAME));
        SetWindowLongW(m_HWND, GWL_EXSTYLE, m_FullScreenInfo.exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
        SetWindowPos(m_HWND, HWND_TOP,
                     monitorInfo.rcMonitor.left,
                     monitorInfo.rcMonitor.top,
                     monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                     monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

        m_Fullscreen = true;
    }

    void WindowWin32::disableFullscreen() {
        if (!m_Fullscreen) {
            return;
        }
        SetWindowLongW(m_HWND, GWL_STYLE, m_FullScreenInfo.style);
        SetWindowLongW(m_HWND, GWL_EXSTYLE, m_FullScreenInfo.exStyle);
        SetWindowPos(m_HWND, nullptr,
                     m_FullScreenInfo.rect.left,
                     m_FullScreenInfo.rect.top,
                     m_FullScreenInfo.rect.right - m_FullScreenInfo.rect.left,
                     m_FullScreenInfo.rect.bottom - m_FullScreenInfo.rect.top,
                     SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        if (m_FullScreenInfo.maximized) {
            ShowWindow(m_HWND, SW_MAXIMIZE);
        }
        m_Fullscreen = false;
    }

    void WindowWin32::showMouse() {
        while (ShowCursor(TRUE) < 0);
    }

    void WindowWin32::hideMouse() {
        while (ShowCursor(FALSE) >= 0);
    }

    void WindowWin32::grabMouse() {
        if (!m_MouseGrabbed){
            RECT windowDimmensions;
            GetClientRect(m_HWND, &windowDimmensions);
            MapWindowPoints(m_HWND, nullptr, reinterpret_cast<LPPOINT>(&windowDimmensions), 2);
            m_MouseGrabbed = ClipCursor(&windowDimmensions);
        }
    }

    void WindowWin32::releaseMouse() {
        if (m_MouseGrabbed){
            ClipCursor(nullptr);
            m_MouseGrabbed = false;
        }
    }

    void WindowWin32::setMousePosition(const I32 mousePosX, const I32 mousePosY) {
        RECT windowRect;
        GetWindowRect(m_HWND, &windowRect);
        const POINT newPos = {mousePosX + windowRect.left, mousePosY + windowRect.top};
        SetCursorPos(newPos.x, newPos.y);
    }

    I32 WindowWin32::getMousePositionX() const {
        POINT mousePos;
        GetCursorPos(&mousePos);
        return mousePos.x;
    }

    I32 WindowWin32::getMousePositionY() const {
        POINT mousePos;
        GetCursorPos(&mousePos);
        return mousePos.y;
    }

    float WindowWin32::getDpiScale() const {
        return 1.0f;
    }

    std::optional<fs::path> WindowWin32::save_file_dialog(const SaveFileDesc &/*desc*/) {
        return {};
    }

    std::optional<fs::path> WindowWin32::open_file_dialog(const OpenFileDesc &/*desc*/) {
        return {};
    }

    std::optional<fs::path> WindowWin32::open_folder_dialog(const OpenFolderDesc &/*desc*/) {
        return {};
    }

    const UP<Context> &WindowWin32::createContext(const ContextDesc &desc) {
        const VL_GRAPHICS_API api = desc.api;
        switch (api) {
            case VL_API_OPENGL: {
                UP<GLPlatformContext> platformContext = createUP<WglPlatformContext>(desc, m_HWND);
                m_Context = createUP<GLContext>(desc, std::move(platformContext));
            }
            default: {
                SPDLOG_LOGGER_ERROR(m_Logger, "Unsupported graphics API {} for Win32Window", api);
            }
        }
        return m_Context;
    }

    void WindowWin32::registerWindowClass() const {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = windowEventProc;
        wc.hInstance = m_HInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = VL_CORE_WIN32_CLASS_NAME;
        wc.hbrBackground = nullptr;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hIcon = nullptr;
        wc.hIconSm = nullptr;
        wc.lpszMenuName = nullptr;

        RegisterClassExW(&wc);
        SPDLOG_LOGGER_TRACE(m_Logger, "Registered Win32 window class");
    }

    void WindowWin32::unregisterWindowClass() const {
        UnregisterClassW(VL_CORE_WIN32_CLASS_NAME, m_HInstance);
    }

    void WindowWin32::registerRawInputDevices() const {
        RAWINPUTDEVICE mouse;
        mouse.usUsagePage = 0x01; //mouse
        mouse.usUsage = 0x02;     //mouse
        mouse.dwFlags = 0;
        mouse.hwndTarget = nullptr; // register to all windows, maybe change this later to only register to this window
        if (!RegisterRawInputDevices(&mouse, 1, sizeof(mouse))) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to register raw input mouse");
        }

        RAWINPUTDEVICE keyboard;
        keyboard.usUsagePage = 0x01; //keyboard
        keyboard.usUsage = 0x06;     //keyboard
        keyboard.dwFlags = 0;
        keyboard.hwndTarget = nullptr; // register to all windows, maybe change this later to only register to this window
        if (!RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard))) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to register raw input keyboard");
        }
    }

    LRESULT CALLBACK WindowWin32::windowEventProc(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam) {
        if (msg == WM_CREATE){
            const auto window = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT *>(lparam)->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, window);
        }
        WindowWin32* window = hwnd ? reinterpret_cast<WindowWin32*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)) : nullptr;
        if (window != nullptr){
            window->handleEvent(msg, wparam, lparam);
        }
        // Prevents win32 from closing the window manually.
        if (msg == WM_CLOSE){
            return 0;
        }
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    void WindowWin32::handleEvent(const UINT msg, const WPARAM wparam, const LPARAM lparam) {
        switch (msg){
            case WM_CREATE: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_OPENED);
                return;
            }
            case WM_DESTROY: {
                PostQuitMessage(0);
                return;
            }
            case WM_CLOSE: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_CLOSED);
                m_Open = false;
                return;
            }
            case WM_SIZE: {
                if (wparam != SIZE_MINIMIZED && !m_Resizing && !m_Moving && m_Open){
                    const I32 newWidth = GET_X_LPARAM(lparam);
                    const I32 newHeight = GET_Y_LPARAM(lparam);
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_RESIZED, newWidth, newHeight);
                }
                break;
            }
            case WM_MOVE: {
                if (!m_Resizing && m_Open){
                    const I32 xPos = GET_X_LPARAM(lparam);
                    const I32 yPos = GET_Y_LPARAM(lparam);
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_MOVED, xPos, yPos);
                }
                break;
            }
            case WM_SETFOCUS: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_GAINED_FOCUS);
                m_Focused = true;
                break;
            }
            case WM_KILLFOCUS: {
                if (m_Open){
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_WINDOW, VL_EVENT_WINDOW_LOST_FOCUS);
                    m_Focused = false;
                }
                break;
            }
            case WM_EXITSIZEMOVE: {
                m_Resizing = true;
                break;
            }
            case WM_ENTERSIZEMOVE: {
                m_Resizing = false;
                break;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                VL_KEYBOARD_KEY key = convertVirtualKey(wparam, lparam);
                m_EventQueue.emplace_back(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_RELEASED, key);
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                if ((HIWORD(lparam) & KF_REPEAT) == 0){
                    VL_KEYBOARD_KEY key = convertVirtualKey(wparam, lparam);
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_PRESSED, key);
                }
                break;
            }
            case WM_SYSCHAR:
            case WM_CHAR: {
                if ((lparam & (1 << 30)) == 0){
                    auto ch = static_cast<char>(wparam);
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_KEYBOARD_KEY_TYPED, ch);
                }
                break;
            }
            case WM_LBUTTONDOWN: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_PRESSED, VL_MOUSE_BUTTON_LEFT);
                break;
            }
            case WM_MBUTTONDOWN: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_PRESSED, VL_MOUSE_BUTTON_MIDDLE);
                break;
            }
            case WM_RBUTTONDOWN: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_PRESSED, VL_MOUSE_BUTTON_RIGHT);
                break;
            }
            case WM_LBUTTONUP: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_RELEASED, VL_MOUSE_BUTTON_LEFT);
                break;
            }
            case WM_MBUTTONUP: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_RELEASED, VL_MOUSE_BUTTON_MIDDLE);
                break;
            }
            case WM_RBUTTONUP: {
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_RELEASED, VL_MOUSE_BUTTON_RIGHT);
                break;
            }
            case WM_XBUTTONDOWN: {
                const VL_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? VL_MOUSE_BUTTON_X1 : VL_MOUSE_BUTTON_X2;
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_PRESSED, button);
                break;
            }
            case WM_XBUTTONUP: {
                const VL_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? VL_MOUSE_BUTTON_X1 : VL_MOUSE_BUTTON_X2;
                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_BUTTON_RELEASED, button);
                break;
            }
            case WM_MOUSELEAVE: {
                if (m_MouseInside){
                    m_MouseInside = false;
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_LEFT);
                }
                break;
            }
            case WM_MOUSEMOVE: {
                auto mouseX = GET_X_LPARAM(lparam);
                auto mouseY = GET_Y_LPARAM(lparam);
                RECT window;
                GetClientRect(m_HWND, &window);
                if ((wparam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0){
                    if (GetCapture() == m_HWND){
                        ReleaseCapture();
                    }
                }
                else if (GetCapture() != m_HWND){
                    SetCapture(m_HWND);
                }
                if ((mouseX < window.left) || (mouseX > window.right) || (mouseY < window.top) || (mouseY > window.bottom)){
                    if (m_MouseInside){
                        m_MouseInside = false;
                        mouseTracker(false);
                        m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_LEFT);
                    }
                }
                else{
                    if (!m_MouseInside){
                        m_MouseInside = true;
                        mouseTracker(true);
                        m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_ENTERED);
                    }
                }
                if (m_MouseInside){
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_MOVED, mouseX, mouseY);
                }
                break;
            }
            case WM_MOUSEWHEEL: {
                if (m_MouseInside){
                    const I32 xOffset = LOWORD(lparam);
                    const I32 yOffset = HIWORD(lparam);
                    m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_MOUSE_SCROLLED, xOffset, yOffset);
                }
                break;
            }
            // raw input from devices
            case WM_INPUT: {
                U32 size;
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1){
                    break;
                }
                std::vector<U8> buffer(size);
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER)) != size){
                    break;
                }
                auto raw = reinterpret_cast<RAWINPUT*>(buffer.data());
                switch (raw->header.dwType) {
                    case RIM_TYPEMOUSE: {
                        if (m_MouseInside){
                            if (raw->data.mouse.lLastX != 0 and raw->data.mouse.lLastY != 0 ){
                                m_EventQueue.emplace_back(VL_EVENT_CLASS_MOUSE, VL_EVENT_RAW_MOUSE_MOVED, raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                            }
                        }
                        break;
                    }
                    case RIM_TYPEKEYBOARD: {
                        auto key = convertVirtualKey(raw->data.keyboard.VKey, raw->data.keyboard.Flags);
                        if (raw->data.keyboard.Flags & RI_KEY_BREAK){
                            m_EventQueue.emplace_back(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_RAW_KEYBOARD_KEY_PRESSED, key);
                        }
                        else{
                            m_EventQueue.emplace_back(VL_EVENT_CLASS_KEYBOARD, VL_EVENT_RAW_KEYBOARD_KEY_RELEASED, key);
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            default: break;
        }
    }

    void WindowWin32::mouseTracker(const bool enableTracking) const {
        TRACKMOUSEEVENT mouseEvent;
        mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        if (enableTracking){
            mouseEvent.dwFlags = TME_LEAVE;
        }
        else{
            mouseEvent.dwFlags = TME_CANCEL;
        }
        mouseEvent.hwndTrack = m_HWND;
        mouseEvent.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&mouseEvent);
    }



}
