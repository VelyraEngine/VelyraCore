#include "../../../Pch.hpp"

#include <VelyraCore/Context/Definitions.hpp>

#include "WglPlatformContext.hpp"
#include "../../../Logging/LoggerNames.hpp"
#include "../../../Logging/Win32Logging.hpp"

namespace Velyra::Core {

    U64 WglPlatformContext::m_WglContextCount = 0;

    WglPlatformContext::WglPlatformContext(const ContextDesc &desc, HWND &hwnd):
    GLPlatformContext(VL_GL_PLATFORM_WGL),
    m_Logger(Utils::getLogger(VL_LOGGER_WGL)),
    m_Hwnd(hwnd),
    m_HDC(nullptr),
    m_Context(nullptr) {
        m_HDC = GetDC(m_Hwnd);
        if (!m_HDC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to get device context: {}", formatWin32ExceptionMessage(GetLastError()));
            return;
        }
        /* -------------------------------------------------------------
       1) Set a minimal legacy pixel format (required ONCE)
       ------------------------------------------------------------- */
        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize      = sizeof(pfd);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = desc.redBits + desc.greenBits + desc.blueBits + desc.alphaBits;
        pfd.cDepthBits = desc.depthBits;
        pfd.cStencilBits = desc.stencilBits;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int legacyPf = ChoosePixelFormat(m_HDC, &pfd);
        if (legacyPf == 0 || !SetPixelFormat(m_HDC, legacyPf, &pfd)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to set legacy pixel format: {}",
                formatWin32ExceptionMessage(GetLastError()));
            ReleaseDC(m_Hwnd, m_HDC);
            return;
        }

        /* -------------------------------------------------------------
           2) Create temporary legacy context
           ------------------------------------------------------------- */
        HGLRC tempContext = wglCreateContext(m_HDC);
        if (!tempContext || !wglMakeCurrent(m_HDC, tempContext)) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to create temporary WGL context");
            ReleaseDC(m_Hwnd, m_HDC);
            return;
        }

        initWGL();

        if (wglCreateContextAttribsARB) {
            const int contextAttribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, VL_GL_MAJOR_VERSION,
                WGL_CONTEXT_MINOR_VERSION_ARB, VL_GL_MINOR_VERSION,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    #if defined(VL_DEBUG)
                WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_DEBUG_BIT_ARB,
    #endif
                0
            };

            HGLRC modernContext = wglCreateContextAttribsARB(m_HDC, nullptr, contextAttribs);

            if (modernContext) {
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(tempContext);

                m_Context = modernContext;
                wglMakeCurrent(m_HDC, m_Context);

                SPDLOG_LOGGER_INFO(m_Logger, "Created OpenGL {}.{} Core context", VL_GL_MAJOR_VERSION, VL_GL_MINOR_VERSION);
            }
            else {
                SPDLOG_LOGGER_WARN(m_Logger, "wglCreateContextAttribsARB failed, falling back to legacy context");
                m_Context = tempContext;
            }
        }
        else {
            SPDLOG_LOGGER_WARN(m_Logger,  "wglCreateContextAttribsARB not available, using legacy context");
            m_Context = tempContext;
        }
    }

    WglPlatformContext::~WglPlatformContext() {

    }

    void WglPlatformContext::setVerticalSynchronisation(bool enable) {

    }

    void WglPlatformContext::swapBuffers() {
        VL_PRECONDITION(m_HDC != nullptr, "WGL device context is null, cannot make current")

        SwapBuffers(m_HDC);
    }

    void WglPlatformContext::makeCurrent() {
        VL_PRECONDITION(m_Context != nullptr, "WGL context is null, cannot make current")
        VL_PRECONDITION(m_HDC != nullptr, "WGL device context is null, cannot make current")

        wglMakeCurrent(m_HDC, m_Context);
    }

    void WglPlatformContext::initWGL() {
        // Although gladLoadWGL can be called multiple times, with different HDCs, we only want to initialize it once
        // for the application since it uses the HDC only to retrieve function pointers.
        if (m_WglContextCount == 0) {
            const int version = gladLoadWGL(m_HDC);
            if (version == 0) {
                SPDLOG_LOGGER_ERROR(m_Logger, "Failed to initialize WGL context");
            } else {
                SPDLOG_LOGGER_INFO(m_Logger, "Initialized WGL context with version: {}", version);
            }
        }
        m_WglContextCount++;
    }

    void WglPlatformContext::terminateWGL() {
        // Prevent underflow
        if (m_WglContextCount == 0) {
            return;
        }
        m_WglContextCount--;
        if (m_WglContextCount == 0) {
            gladUnloadWGL();
        }
    }


}
