#pragma once

#include "GLPlatformContext.hpp"

namespace Velyra::Core {

    class WglPlatformContext: public GLPlatformContext {
    public:
        WglPlatformContext(const ContextDesc& desc, HWND& hwnd);

        ~WglPlatformContext() override;

        void setVerticalSynchronisation(bool enable) override;

        void swapBuffers() override;

        void makeCurrent() override;

    private:

        void initWGL();

        void terminateWGL();

    private:
        static U64 m_WglContextCount;

        Utils::LogPtr m_Logger;
        HWND& m_Hwnd;
        HDC m_HDC;
        HGLRC m_Context;
    };
}