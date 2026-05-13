#include "../../Pch.hpp"

#include "GLState.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLState::GLState():
    ApiState(VL_API_OPENGL),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        capture();
    }

    bool GLState::isEqual(const ApiState &other) const {
        auto otherState = dynamic_cast<const GLState*>(&other);
        if (!otherState) {
            SPDLOG_LOGGER_CRITICAL(m_Logger, "Attempted to compare GLState with non-GLState!");
            return false;
        }
        bool equal = true;
        if (std::memcmp(m_Viewport, otherState->m_Viewport, sizeof(m_Viewport)) != 0) {
            SPDLOG_LOGGER_WARN(m_Logger, "Viewport state mismatch! This: X={}, Y={}, W={}, H={} | Other: X={}, Y={}, W={}, H={}",
                               m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3],
                               otherState->m_Viewport[0], otherState->m_Viewport[1], otherState->m_Viewport[2], otherState->m_Viewport[3]);
            equal = false;
        }
        if (std::memcmp(m_ColorMask, otherState->m_ColorMask, sizeof(m_ColorMask)) != 0) {
            SPDLOG_LOGGER_WARN(m_Logger, "Color mask state mismatch! This: R={}, G={}, B={}, A={} | Other: R={}, G={}, B={}, A={}",
                               m_ColorMask[0], m_ColorMask[1], m_ColorMask[2], m_ColorMask[3],
                               otherState->m_ColorMask[0], otherState->m_ColorMask[1], otherState->m_ColorMask[2], otherState->m_ColorMask[3]);
            equal = false;
        }
        if (m_ScissorEnabled != otherState->m_ScissorEnabled) {
            SPDLOG_LOGGER_WARN(m_Logger, "Scissor test state mismatch! This: {} | Other: {}",
                               m_ScissorEnabled ? "Enabled" : "Disabled",
                               otherState->m_ScissorEnabled ? "Enabled" : "Disabled");
            equal = false;
        }
        if (std::memcmp(m_ScissorBox, otherState->m_ScissorBox, sizeof(m_ScissorBox)) != 0) {
            SPDLOG_LOGGER_WARN(m_Logger, "Scissor box state mismatch! This: X={}, Y={}, W={}, H={} | Other: X={}, Y={}, W={}, H={}",
                               m_ScissorBox[0], m_ScissorBox[1], m_ScissorBox[2], m_ScissorBox[3],
                               otherState->m_ScissorBox[0], otherState->m_ScissorBox[1], otherState->m_ScissorBox[2], otherState->m_ScissorBox[3]);
            equal = false;
        }
        if (m_DepthTestEnabled != otherState->m_DepthTestEnabled) {
            SPDLOG_LOGGER_WARN(m_Logger, "Depth test state mismatch! This: {} | Other: {}",
                               m_DepthTestEnabled ? "Enabled" : "Disabled",
                               otherState->m_DepthTestEnabled ? "Enabled" : "Disabled");
            equal = false;
        }
        if (m_DepthFunc != otherState->m_DepthFunc) {
            SPDLOG_LOGGER_WARN(m_Logger, "Depth func state mismatch! This: {} | Other: {}",
                               m_DepthFunc,
                               otherState->m_DepthFunc);
            equal = false;
        }
        if (m_DepthMask != otherState->m_DepthMask) {
            SPDLOG_LOGGER_WARN(m_Logger, "Depth mask state mismatch! This: {} | Other: {}",
                               m_DepthMask ? "True" : "False",
                               otherState->m_DepthMask ? "True" : "False");
            equal = false;
        }
        if (m_BlendEnabled != otherState->m_BlendEnabled) {
            SPDLOG_LOGGER_WARN(m_Logger, "Blend state mismatch! This: {} | Other: {}",
                               m_BlendEnabled ? "Enabled" : "Disabled",
                               otherState->m_BlendEnabled ? "Enabled" : "Disabled");
            equal = false;
        }
        if (m_CullFaceEnabled != otherState->m_CullFaceEnabled) {
            SPDLOG_LOGGER_WARN(m_Logger, "Cull face state mismatch! This: {} | Other: {}",
                               m_CullFaceEnabled ? "Enabled" : "Disabled",
                               otherState->m_CullFaceEnabled ? "Enabled" : "Disabled");
            equal = false;
        }
        if (m_FrontFace != otherState->m_FrontFace) {
            SPDLOG_LOGGER_WARN(m_Logger, "Front face state mismatch! This: {} | Other: {}",
                               m_FrontFace == GL_FRONT ? "GL_FRONT" : "GL_BACK",
                               otherState->m_FrontFace == GL_FRONT ? "GL_FRONT" : "GL_BACK");
            equal = false;
        }
        if (m_CullFace != otherState->m_CullFace) {
            SPDLOG_LOGGER_WARN(m_Logger, "Cull face mode state mismatch! This: {} | Other: {}",
                               m_CullFace == GL_BACK ? "GL_BACK" : "GL_FRONT",
                               otherState->m_CullFace == GL_BACK ? "GL_BACK" : "GL_FRONT");
            equal = false;
        }
        if (m_ArrayBuffer != otherState->m_ArrayBuffer) {
            SPDLOG_LOGGER_WARN(m_Logger, "Array buffer state mismatch! This: {} | Other: {}",
                               m_ArrayBuffer,
                               otherState->m_ArrayBuffer);
            equal = false;
        }
        if (m_ElementArrayBuffer != otherState->m_ElementArrayBuffer) {
            SPDLOG_LOGGER_WARN(m_Logger, "Element array buffer state mismatch! This: {} | Other: {}",
                               m_ElementArrayBuffer,
                               otherState->m_ElementArrayBuffer);
            equal = false;
        }
        if (m_UniformBuffer != otherState->m_UniformBuffer) {
            SPDLOG_LOGGER_WARN(m_Logger, "Uniform buffer state mismatch! This: {} | Other: {}",
                               m_UniformBuffer,
                               otherState->m_UniformBuffer);
            equal = false;
        }
        if (m_DrawFBO != otherState->m_DrawFBO) {
            SPDLOG_LOGGER_WARN(m_Logger, "Draw FBO state mismatch! This: {} | Other: {}",
                               m_DrawFBO,
                               otherState->m_DrawFBO);
            equal = false;
        }
        if (m_ReadFBO != otherState->m_ReadFBO) {
            SPDLOG_LOGGER_WARN(m_Logger, "Read FBO state mismatch! This: {} | Other: {}",
                               m_ReadFBO,
                               otherState->m_ReadFBO);
            equal = false;
        }
        if (m_Vao != otherState->m_Vao) {
            SPDLOG_LOGGER_WARN(m_Logger, "VAO state mismatch! This: {} | Other: {}",
                               m_Vao,
                               otherState->m_Vao);
            equal = false;
        }
        if (m_ShaderProgram != otherState->m_ShaderProgram) {
            SPDLOG_LOGGER_WARN(m_Logger, "Shader program state mismatch! This: {} | Other: {}",
                               m_ShaderProgram,
                               otherState->m_ShaderProgram);
            equal = false;
        }
        if (m_ActiveTexture != otherState->m_ActiveTexture) {
            SPDLOG_LOGGER_WARN(m_Logger, "Active texture unit state mismatch! This: {} | Other: {}",
                               m_ActiveTexture,
                               otherState->m_ActiveTexture);
            equal = false;
        }
        for (int i = 0; i < 32; ++i) {
            if (m_BoundTextures[i] != otherState->m_BoundTextures[i]) {
                SPDLOG_LOGGER_WARN(m_Logger, "Texture unit {} bound texture state mismatch! This: {} | Other: {}",
                                   i,
                                   m_BoundTextures[i],
                                   otherState->m_BoundTextures[i]);
                equal = false;
            }
        }
        for (int i = 0; i < 32; ++i) {
            if (m_UniformBindings[i] != otherState->m_UniformBindings[i]) {
                SPDLOG_LOGGER_WARN(m_Logger, "Uniform buffer binding {} state mismatch! This: {} | Other: {}",
                                   i,
                                   m_UniformBindings[i],
                                   otherState->m_UniformBindings[i]);
                equal = false;
            }
        }
        return equal;
    }

    void GLState::log() const {
        SPDLOG_LOGGER_INFO(m_Logger, "OpenGL State:");
        SPDLOG_LOGGER_INFO(m_Logger, "Viewport: [{}, {}, {}, {}]", m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3]);
        SPDLOG_LOGGER_INFO(m_Logger, "Color Mask: R={}, G={}, B={}, A={}", m_ColorMask[0], m_ColorMask[1], m_ColorMask[2], m_ColorMask[3]);
        SPDLOG_LOGGER_INFO(m_Logger, "Scissor Test: {}", m_ScissorEnabled ? "Enabled" : "Disabled");
        SPDLOG_LOGGER_INFO(m_Logger, "Scissor Box: [{}, {}, {}, {}]", m_ScissorBox[0], m_ScissorBox[1], m_ScissorBox[2], m_ScissorBox[3]);
        SPDLOG_LOGGER_INFO(m_Logger, "Depth Test: {}", m_DepthTestEnabled ? "Enabled" : "Disabled");
        SPDLOG_LOGGER_INFO(m_Logger, "Depth Func: {}", m_DepthFunc);
        SPDLOG_LOGGER_INFO(m_Logger, "Depth Mask: {}", m_DepthMask ? "True" : "False");
        SPDLOG_LOGGER_INFO(m_Logger, "Blend: {}", m_BlendEnabled ? "Enabled" : "Disabled");
        SPDLOG_LOGGER_INFO(m_Logger, "Cull Face: {}", m_CullFaceEnabled ? "Enabled" : "Disabled");
        SPDLOG_LOGGER_INFO(m_Logger, "Front Face: {}", m_FrontFace == GL_FRONT ? "GL_FRONT" : "GL_BACK");
        SPDLOG_LOGGER_INFO(m_Logger, "Cull Face Mode: {}", m_CullFace == GL_BACK ? "GL_BACK" : "GL_FRONT");
        SPDLOG_LOGGER_INFO(m_Logger, "Array Buffer: {}", m_ArrayBuffer);
        SPDLOG_LOGGER_INFO(m_Logger, "Element Array Buffer: {}", m_ElementArrayBuffer);
        SPDLOG_LOGGER_INFO(m_Logger, "Uniform Buffer: {}", m_UniformBuffer);
        SPDLOG_LOGGER_INFO(m_Logger, "Draw FBO: {}", m_DrawFBO);
        SPDLOG_LOGGER_INFO(m_Logger, "Read FBO: {}", m_ReadFBO);
        SPDLOG_LOGGER_INFO(m_Logger, "VAO: {}", m_Vao);
        SPDLOG_LOGGER_INFO(m_Logger, "Shader Program: {}", m_ShaderProgram);
        SPDLOG_LOGGER_INFO(m_Logger, "Active Texture Unit: {}", m_ActiveTexture);
        for (int i = 0; i < 32; ++i) {
            SPDLOG_LOGGER_INFO(m_Logger, "Texture Unit {}: Bound Texture ID {}", i, m_BoundTextures[i]);
        }
        for (int i = 0; i < 32; ++i) {
            SPDLOG_LOGGER_INFO(m_Logger, "Uniform Buffer Binding {}: Bound Buffer ID {}", i, m_UniformBindings[i]);
        }

    }

    void GLState::capture() {
        glGetIntegerv(GL_VIEWPORT, m_Viewport);
        glGetBooleanv(GL_COLOR_WRITEMASK, m_ColorMask);

        m_ScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
        glGetIntegerv(GL_SCISSOR_BOX, m_ScissorBox);

        m_DepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_DEPTH_FUNC, &m_DepthFunc);
        glGetBooleanv(GL_DEPTH_WRITEMASK, &m_DepthMask);
        m_BlendEnabled = glIsEnabled(GL_BLEND);
        m_CullFaceEnabled = glIsEnabled(GL_CULL_FACE);
        glGetIntegerv(GL_FRONT_FACE, &m_FrontFace);
        glGetIntegerv(GL_CULL_FACE_MODE, &m_CullFace);

        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_DrawFBO);
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &m_ReadFBO);

        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_ArrayBuffer);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_ElementArrayBuffer);
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &m_UniformBuffer);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_Vao);
        glGetIntegerv(GL_CURRENT_PROGRAM, &m_ShaderProgram);

        glGetIntegerv(GL_ACTIVE_TEXTURE, &m_ActiveTexture);
        for (int i = 0; i < 32; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_BoundTextures[i]);
        }
        // Restore the active texture unit
        glActiveTexture(m_ActiveTexture);

        for (int i = 0; i < 32; ++i){
            glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, i, &m_UniformBindings[i]);
        }
    }
}

