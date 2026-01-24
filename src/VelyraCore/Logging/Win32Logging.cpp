#include "../Pch.hpp"

#include "Win32Logging.hpp"
#include "LoggerNames.hpp"

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

    bool decodeHRESULT(const HRESULT hr) {
        if (SUCCEEDED(hr)) {
            return true;
        }
        const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_WIN32);

        const _com_error comError(hr);
        const char* comMessage = comError.ErrorMessage();
        const DWORD win32Code = HRESULT_CODE(hr);
        const std::string win32Message = formatWin32ExceptionMessage(win32Code);
        SPDLOG_LOGGER_ERROR(
            logger,
            "HRESULT failed: 0x{:08X}, COM: {}, Win32(decoded): {}",
            static_cast<uint32_t>(hr),
            comMessage,
            win32Message
        );
        return false;
    }

}
