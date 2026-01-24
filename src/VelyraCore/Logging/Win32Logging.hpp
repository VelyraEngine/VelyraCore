#pragma once

#include <VelyraUtils/Logging.hpp>

namespace Velyra::Core {

    std::string formatWin32ExceptionMessage(DWORD exceptionCode);

    bool decodeHRESULT(HRESULT hr);

}

#if defined(VL_DEBUG)

#define VL_CHECK_HR(x) \
    do { \
        HRESULT hr__ = (x); \
        Velyra::Core::decodeHRESULT(hr__); \
    } while (0)

#else

#define VL_CHECK_HR(x) (x)

#endif