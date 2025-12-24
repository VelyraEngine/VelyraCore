#include "../../Pch.hpp"

#include "GLShaderModule.hpp"
#include "../../../Logging/LoggerNames.hpp"
#include "Internal/Translations.hpp"

namespace Velyra::Core {

    GLShaderModule::GLShaderModule(const ShaderModuleDesc &desc):
    ShaderModule(desc),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL))
    {
        createShader(desc.code);
    }

    GLShaderModule::GLShaderModule(const ShaderModuleFileDesc &desc):
    ShaderModule(desc),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        if (!fs::exists(desc.filePath)) {
            SPDLOG_LOGGER_WARN(m_Logger, "Shader not found on disk, location: {}", desc.filePath.string());
            return;
        }
        std::string code = loadShaderFromFile(desc.filePath);
        createShader(code);
    }

    GLShaderModule::~GLShaderModule() {
        glDeleteShader(m_ShaderModuleID);
    }

    U64 GLShaderModule::getIdentifier() const {
        return m_ShaderModuleID;
    }

    std::string GLShaderModule::loadShaderFromFile(const fs::path &filePath) const {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Failed to open shader file: {}", filePath.string());
            return "";
        }

        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }


    void GLShaderModule::createShader(const std::string &code) {
        if (code.empty()) {
            SPDLOG_LOGGER_WARN(m_Logger, "Shader code is empty, cannot create shader module");
            return;
        }
        m_ShaderModuleID = glCreateShader(gelGLShaderType(m_ShaderType));
        const GLchar* codePtr = code.c_str();
        // It if same to pass nulltr for length, as the strings are null-terminated
        // Guaranteed by the std::string container
        glShaderSource(m_ShaderModuleID, 1, &codePtr, nullptr);
        glCompileShader(m_ShaderModuleID);
        GLint success;
        glGetShaderiv(m_ShaderModuleID, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(m_ShaderModuleID, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(logLength, '\0');
            glGetShaderInfoLog(m_ShaderModuleID, logLength, nullptr, infoLog.data());

            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to compile shader! ID = {}, type = {}, error = {}", m_ShaderModuleID, m_ShaderType, infoLog);
        }
    }


}
