#pragma once

#include "GLPlatformContext.hpp"

namespace Velyra::Core {

    class WglPlatformContext: public GLPlatformContext {
    public:
        WglPlatformContext(const ContextDesc& desc, HWND& hwnd);

        ~WglPlatformContext() override;

        void setVerticalSynchronisation(bool enable) override;

        bool isVerticalSynchronisationEnabled() const override;

        void swapBuffers() override;

        void makeCurrent() override;

        void initPlatformImGui(const ImGuiDesc& desc) override;

        void terminatePlatformImGui() override;

        void onPlatformImGuiBegin() override;

        void onPlatformImGuiEnd() override;

    private:

        void initWGL() const;

        static void terminateWGL();

        HGLRC createModernContext(HGLRC tempContext) const;

        void loadExtensions();

        bool isExtensionSupported(std::string_view extensionName) const;

    private:
        static U64 m_WglContextCount;

        Utils::LogPtr m_Logger;
        HWND& m_Hwnd;
        HDC m_HDC;
        HGLRC m_Context;

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;
    };
}