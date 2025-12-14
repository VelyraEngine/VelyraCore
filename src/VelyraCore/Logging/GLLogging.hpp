#pragma once

#include <VelyraUtils/Logging.hpp>

namespace Velyra::Core {

    void formatOpenGLError(unsigned int error);

    void clearOpenGLErrors();

    void logOpenGLErrors();

}

#if defined(VL_DEBUG)

#define VL_CORE_OPENGL_CLEAR_ERRORS() Velyra::Core::clearOpenGLErrors();
#define VL_CORE_OPENGL_LOG_ERRORS() Velyra::Core::logOpenGLErrors();
#define VL_CORE_OPENGL_CALL(x) \
    Velyra::Core::clearOpenGLErrors(); \
    x; \
    Velyra::Core::logOpenGLErrors();

#else

#define VL_CORE_OPENGL_CLEAR_ERRORS()
#define VL_CORE_OPENGL_LOG_ERRORS()
#define VL_CORE_OPENGL_CALL(x) x;

#endif