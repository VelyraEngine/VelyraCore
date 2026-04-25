#include "../../Pch.hpp"

#include "GLDepthStencilState.hpp"
#include "Internal/GLTranslations.hpp"

#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLDepthStencilState::GLDepthStencilState(const DepthStencilStateDesc& desc, const Device& device) :
    DepthStencilState(desc, device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_GLDepthComparisonFunc(getGLComparisonFunc(desc.depthComparisonFunc)),
    m_GLStencilComparisonFunc(getGLComparisonFunc(desc.stencilComparisonFunc)),
    m_GLStencilFail(getGLStencilFunc(desc.stencilFail)),
    m_GLStencilPassDepthFail(getGLStencilFunc(desc.stencilPassDepthFail)),
    m_GLStencilPass(getGLStencilFunc(desc.stencilPass)) {
        SPDLOG_LOGGER_TRACE(
            m_Logger,
            "Created GLDepthStencilState with depth test {}, depth func {}, depth mask {}, stencil test {}, stencil func {}, stencil mask {}, stencil fail op {}, stencil pass op {}, stencil pass depth fail op {}",
            m_EnableDepthTest ? "enabled" : "disabled", m_DepthFunc, m_DepthMask,
            m_EnableStencilTest ? "enabled" : "disabled", m_StencilFunc, m_StencilMask,
            m_StencilFail, m_StencilPass, m_StencilPassDepthFail);

    }

    GLDepthStencilState::~GLDepthStencilState() = default;

    void GLDepthStencilState::bind() const {
        if (m_DepthTestEnabled){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GLDepthComparisonFunc);
            glDepthMask(m_DepthMask);
        }
        if (m_StencilTestEnabled) {
            glEnable(GL_STENCIL_TEST);
            SPDLOG_LOGGER_CRITICAL(m_Logger, "Stencil Comparison function not set correctly!");
            // glStencilFunc(m_GLStencilComparisonFunc, m_ClearStencil, m_StencilMask);
            glStencilOp(m_GLStencilFail, m_GLStencilPassDepthFail, m_GLStencilPass);
        }
        SPDLOG_LOGGER_TRACE(m_Logger, "Bound GLDepthStencilState with depth test {}, depth func {}, depth mask {}, stencil test {}, stencil func {}, stencil mask {}, stencil fail op {}, stencil pass op {}, stencil pass depth fail op {}",
            m_DepthTestEnabled ? "enabled" : "disabled", m_DepthComparisonFunc, m_DepthMask,
            m_StencilTestEnabled ? "enabled" : "disabled", m_StencilComparisonFunc, m_StencilMask,
            m_StencilFailOp, m_StencilPassOp, m_StencilPassDepthFailOp);
    }

    void GLDepthStencilState::unbind() const {
        if (m_DepthTestEnabled){
            glDisable(GL_DEPTH_TEST);
        }
        if (m_StencilTestEnabled) {
            glDisable(GL_STENCIL_TEST);
        }
    }

    void GLDepthStencilState::setDepthTestEnabled(const bool enabled) {
        m_DepthTestEnabled = enabled;
        SPDLOG_LOGGER_TRACE(m_Logger, "Set depth test to {}", enabled ? "enabled" : "disabled");
    }

    void GLDepthStencilState::setDepthComparisonFunc(const VL_COMPARISON_FUNC func) {
        m_DepthComparisonFunc = func;
        m_GLDepthComparisonFunc = getGLComparisonFunc(func);
        SPDLOG_LOGGER_TRACE(m_Logger, "Set depth comparison function to {}", func);
    }

    void GLDepthStencilState::setDepthMask(const VL_DEPTH_MASK mask) {
        m_DepthMask = mask;
        SPDLOG_LOGGER_TRACE(m_Logger, "Set depth mask to {}", mask);
    }

    void GLDepthStencilState::setStencilTestEnabled(const bool enabled) {
        m_StencilTestEnabled = enabled;
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil test to {}", enabled ? "enabled" : "disabled");
    }

    void GLDepthStencilState::setStencilComparisonFunc(const VL_COMPARISON_FUNC func) {
        m_StencilComparisonFunc = func;
        m_GLStencilComparisonFunc = getGLComparisonFunc(func);
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil comparison function to {}", func);
    }

    void GLDepthStencilState::setStencilMask(const U32 mask) {
        m_StencilMask = mask;
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil mask to {}", mask);
    }

    void GLDepthStencilState::setStencilFailOp(const VL_STENCIL_FUNC func) {
        m_StencilFailOp = func;
        m_GLStencilFail = getGLStencilFunc(func);
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil fail operation to {}", func);
    }

    void GLDepthStencilState::setStencilPassOp(const VL_STENCIL_FUNC func) {
        m_StencilPassOp = func;
        m_GLStencilPass = getGLStencilFunc(func);
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil pass operation to {}", func);
    }

    void GLDepthStencilState::setStencilPassDepthFailOp(const VL_STENCIL_FUNC func) {
        m_StencilPassDepthFailOp = func;
        m_GLStencilPassDepthFail = getGLStencilFunc(func);
        SPDLOG_LOGGER_TRACE(m_Logger, "Set stencil pass depth fail operation to {}", func);
    }
}
