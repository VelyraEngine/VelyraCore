#include "../../Pch.hpp"

#include "Glfw3Window.hpp"
#include "Glfw3Utils.hpp"

namespace Velyra::Core {

    Size Glfw3Window::m_GlfwWindowCount = 0;

    Glfw3Window::Glfw3Window(const WindowDesc &desc) {

    }

    Glfw3Window::~Glfw3Window() {

    }

    I32 Glfw3Window::getPositionX() const {
        return 0;
    }

    I32 Glfw3Window::getPositionY() const {
        return 0;
    }

    U32 Glfw3Window::getWidth() const {
        return 0;
    }

    U32 Glfw3Window::getHeight() const {
        return 0;
    }

    std::string Glfw3Window::getTitle() const {
        return "";
    }

    bool Glfw3Window::isOpen() const {
        return false;
    }

    bool Glfw3Window::isFullscreen() const {
        return false;
    }

    bool Glfw3Window::isFocused() const {
        return false;
    }

    bool Glfw3Window::hasEvent() const {
        return false;
    }

    void Glfw3Window::pollEvents() {

    }

    Event Glfw3Window::getNextEvent() {
        return {};
    }

    void Glfw3Window::close() {

    }

    void Glfw3Window::setPosition(const I32 xPosition, const I32 yPosition) {

    }

    void Glfw3Window::setSize(const U32 width, const U32 height) {

    }

    void Glfw3Window::setTitle(const std::string &title) {

    }

    void Glfw3Window::show() {

    }

    void Glfw3Window::hide() {

    }

    void Glfw3Window::enableFullscreen() {

    }

    void Glfw3Window::disableFullscreen() {

    }

    void Glfw3Window::showMouse() {

    }

    void Glfw3Window::hideMouse() {

    }

    void Glfw3Window::grabMouse() {

    }

    void Glfw3Window::releaseMouse() {

    }

    void Glfw3Window::setMousePosition(const I32 mousePosX, const I32 mousePosY) {

    }

    I32 Glfw3Window::getMousePositionX() const {
        return 0;
    }

    I32 Glfw3Window::getMousePositionY() const {
        return 0;
    }

    float Glfw3Window::getDpiScale() const {
        return 1.0f;
    }

    std::optional<fs::path> Glfw3Window::saveFileDialog(const SaveFileDesc &desc) {
        return std::nullopt;
    }

    std::optional<fs::path> Glfw3Window::openFileDialog(const OpenFileDesc &desc) {
        return std::nullopt;
    }

    std::optional<fs::path> Glfw3Window::openFolderDialog(const OpenFolderDesc &desc) {
        return std::nullopt;
    }

    const UP<Context> &Glfw3Window::createContext(const ContextDesc &desc) {
        return m_Context;
    }

}
