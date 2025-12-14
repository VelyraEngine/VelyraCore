#include "../Pch.hpp"

#include "Win32Logging.hpp"

namespace Velyra::Core {

    std::string formatWin32ExceptionMessage(const DWORD exceptionCode) {
        LPSTR message = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            exceptionCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&message),
            0,
            nullptr
        );

        if (message) {
            std::string formattedMessage = std::string(message) + " (0x" + std::to_string(exceptionCode) + ")";
            LocalFree(message);
            return formattedMessage;
        }
        return "Unknown error code (0x" + std::to_string(exceptionCode) + ")";
    }

}
