#include "../../Pch.hpp"

#include "GLSampler.hpp"
#include "Internal/GLTranslations.hpp"
#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLSampler::GLSampler(const Device& device, const SamplerDesc& desc):
    Sampler(device, desc),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_SamplerID(0) {
        glCreateSamplers(1, &m_SamplerID);

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGLTextureWrap(m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGLTextureWrap(m_WrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGLTextureWrap(m_WrapW));

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGLTextureFilter(m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGLTextureFilter(m_MagFilter));

        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, m_MinLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, m_MaxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, m_LODBias);

        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, m_BorderColor);
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(getGLComparisonFunc(m_ComparisonFunc)));

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLSampler with ID {}", m_SamplerID);
    }

    GLSampler::~GLSampler() {
        glDeleteSamplers(1, &m_SamplerID);

        SPDLOG_LOGGER_TRACE(m_Logger, "Destroyed GLSampler with ID {}", m_SamplerID);
    }

    void GLSampler::bindShaderResource(const U32 slot) {
        glBindSampler(slot, m_SamplerID);

        SPDLOG_LOGGER_TRACE(m_Logger, "Bound GLSampler with ID {} to slot {}", m_SamplerID, slot);
    }

    void GLSampler::setFilter(const VL_TEXTURE_FILTER minFilter, const VL_TEXTURE_FILTER magFilter) {
        m_MinFilter = minFilter;
        m_MagFilter = magFilter;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGLTextureFilter(m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGLTextureFilter(m_MagFilter));
    }

    void GLSampler::setWrap(const VL_TEXTURE_WRAP wrapU, const VL_TEXTURE_WRAP wrapV, const VL_TEXTURE_WRAP wrapW) {
        m_WrapU = wrapU;
        m_WrapV = wrapV;
        m_WrapW = wrapW;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGLTextureWrap(m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGLTextureWrap(m_WrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGLTextureWrap(m_WrapW));
    }

    void GLSampler::setComparisonFunc(const VL_COMPARISON_FUNC comparisonFunc) {
        m_ComparisonFunc = comparisonFunc;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(getGLComparisonFunc(m_ComparisonFunc)));
    }

    void GLSampler::setBorderColor(const float r, const float g, const float b, const float a) {
        m_BorderColor[0] = r;
        m_BorderColor[1] = g;
        m_BorderColor[2] = b;
        m_BorderColor[3] = a;
        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, m_BorderColor);
    }

    void GLSampler::setLOD(const float minLOD, const float maxLOD, const float LODBias) {
        m_MinLOD = minLOD;
        m_MaxLOD = maxLOD;
        m_LODBias = LODBias;
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, m_MinLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, m_MaxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, m_LODBias);
    }

    U64 GLSampler::getIdentifier() {
        return m_SamplerID;
    }
}