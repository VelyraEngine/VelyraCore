#pragma once

#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraCore/Window/Event.hpp>
#include <optional>

VL_ENUM(VL_WINDOW_STYLE, int,
    VL_WINDOW_STYLE_POPUP       = 0x01,   // 0000 0001
    VL_WINDOW_STYLE_TITLEBAR    = 0x02,    // 0000 0010
    VL_WINDOW_STYLE_RESIZE      = 0x04,    // 0000 0100
    VL_WINDOW_STYLE_CLOSE       = 0x08,    // 0000 1000
    VL_WINDOW_STYLE_FILE_DROP   = 0x10,    // 0001 0000
    VL_WINDOW_STYLE_SUNKEN_EDGE = 0x20,    // 0010 0000
    VL_WINDOW_STYLE_RAISED_EDGE = 0x40,    // 0100 0000
    VL_WINDOW_STYLE_DEFAULT     = 0x1e    // 0001 1110
    );

VL_ENUM(VL_WINDOW_ICON, int,
    VL_WINDOW_ICON_NONE  = 0x00,
    VL_WINDOW_ICON_SMALL = 0x01,
    VL_WINDOW_ICON_BIG   = 0x02,
    VL_WINDOW_ICON_BOTH  = 0x03
    );

namespace Velyra::Core {

    namespace fs = std::filesystem;

    struct WindowDesc {
        I32 xPosition         = 0;
        I32 yPosition         = 0;
        U32 width             = VL_DEFAULT_WIDTH;
        U32 height            = VL_DEFAULT_HEIGHT;
        std::string title     = "VelyraCore";
        VL_WINDOW_STYLE style = VL_WINDOW_STYLE_DEFAULT;
    };

    struct OpenFileDesc {
        std::string title;
        std::string defaultPath;
        std::vector<std::string> filterPatterns;
        std::string filterDescription;
        bool allowMultipleSelects = false;
    };

    struct SaveFileDesc {
        std::string title;
        std::string defaultPath;
        std::vector<std::string> filterPatterns;
        std::string filterDescription;
    };

    struct OpenFolderDesc {
        std::string title;
        std::string defaultPath;
    };

    class VL_API Window {
    public:
        virtual ~Window() = default;

        virtual I32 getPositionX() const = 0;

        virtual I32 getPositionY() const = 0;

        virtual U32 getWidth() const = 0;

        virtual U32 getHeight() const = 0;

        virtual std::string getTitle() const = 0;

        virtual bool isOpen() const = 0;

        virtual bool isFullscreen() const = 0;

        virtual bool isFocused() const = 0;

        virtual bool hasEvent() const = 0;

        virtual void pollEvents() = 0;

        virtual Event getNextEvent() = 0;

        virtual void close() = 0;

        virtual void setPosition(I32 xPosition, I32 yPosition) = 0;

        virtual void setSize(U32 width, U32 height) = 0;

        virtual void setTitle(const std::string& title) = 0;

        virtual void show() = 0;

        virtual void hide() = 0;

        virtual void enableFullscreen() = 0;

        virtual void disableFullscreen() = 0;

        virtual void showMouse() = 0;

        virtual void hideMouse() = 0;

        virtual void grabMouse() = 0;

        virtual void releaseMouse() = 0;

        virtual void setMousePosition(I32 mousePosX, I32 mousePosY) = 0;

        virtual I32 getMousePositionX() const = 0;

        virtual I32 getMousePositionY() const = 0;

        virtual float getDpiScale() const = 0;

        virtual std::optional<fs::path> save_file_dialog(const SaveFileDesc& desc) = 0;

        virtual std::optional<fs::path> open_file_dialog(const OpenFileDesc& desc) = 0;

        virtual std::optional<fs::path> open_folder_dialog(const OpenFolderDesc& desc) = 0;

    protected:
        Window() = default;

    };
}