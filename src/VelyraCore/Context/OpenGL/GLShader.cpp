#include "../../Pch.hpp"

#include "GLShader.hpp"
#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLShader::GLShader(const ShaderDesc &desc):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        createProgram(desc);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_ShaderID);
    }

    void GLShader::bind() {
        SPDLOG_LOGGER_TRACE(m_Logger, "Binding shader: {}", m_ShaderID);
        glUseProgram(m_ShaderID);
    }

    U64 GLShader::getIdentifier() const {
        return m_ShaderID;
    }

    void GLShader::createProgram(const ShaderDesc &desc) {
        if (m_ShaderID != 0) {
            glDeleteProgram(m_ShaderID);
        }
        if (desc.vertexShader == nullptr) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Missing Vertex Shader Module");
            return;
        }
        if (desc.fragmentShader == nullptr) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Missing Fragment Shader Module");
            return;
        }

        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, static_cast<GLuint>(desc.vertexShader->getIdentifier()));
        glAttachShader(m_ShaderID, static_cast<GLuint>(desc.fragmentShader->getIdentifier()));

        glLinkProgram(m_ShaderID);
        GLint success;
        glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(static_cast<std::size_t>(logLength), '\0');
            // Safe to pass nullptr for length as std::string is null-terminated
            glGetProgramInfoLog(m_ShaderID, logLength, nullptr, log.data());

            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to link shader program ({}), error: {}", m_ShaderID, log);
        }
        else {
            SPDLOG_LOGGER_TRACE(m_Logger, "Shader program {} created!", m_ShaderID);
        }
    }
}
