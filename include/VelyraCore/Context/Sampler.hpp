#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API SamplerDesc {
        VL_TEXTURE_FILTER minFilter   = VL_TEXTURE_FILTER_LINEAR;
        VL_TEXTURE_FILTER magFilter   = VL_TEXTURE_FILTER_LINEAR;
        VL_TEXTURE_WRAP wrapU  = VL_TEXTURE_WRAP_REPEAT;
        VL_TEXTURE_WRAP wrapV  = VL_TEXTURE_WRAP_REPEAT;
        VL_TEXTURE_WRAP wrapW  = VL_TEXTURE_WRAP_REPEAT;
        VL_COMPARISON_FUNC comparisonFunc = VL_COMPARISON_FUNC_NEVER;
        float borderColorR = 0.0f;
        float borderColorG = 0.0f;
        float borderColorB = 0.0f;
        float borderColorA = 1.0f;
        float minLOD = 0.0f;
        float maxLOD = std::numeric_limits<float>::max();
        float LODBias = 0.0f;
    };

    class VL_API Sampler {
    public:
        explicit Sampler(const Device& device, const SamplerDesc& desc):
            m_Device(device),
            m_MinFilter(desc.minFilter),
            m_MagFilter(desc.magFilter),
            m_WrapU(desc.wrapU),
            m_WrapV(desc.wrapV),
            m_WrapW(desc.wrapW),
            m_ComparisonFunc(desc.comparisonFunc),
            m_BorderColor{ desc.borderColorR, desc.borderColorG, desc.borderColorB, desc.borderColorA },
            m_MinLOD(desc.minLOD),
            m_MaxLOD(desc.maxLOD),
            m_LODBias(desc.LODBias){
        }

        virtual ~Sampler() = default;

        virtual void bindShaderResource(U32 slot) = 0;

        virtual void setFilter(VL_TEXTURE_FILTER minFilter, VL_TEXTURE_FILTER magFilter) = 0;

        virtual void setWrap(VL_TEXTURE_WRAP wrapU, VL_TEXTURE_WRAP wrapV, VL_TEXTURE_WRAP wrapW) = 0;

        virtual void setComparisonFunc(VL_COMPARISON_FUNC comparisonFunc) = 0;

        virtual void setBorderColor(float r, float g, float b, float a) = 0;

        virtual void setLOD(float minLOD, float maxLOD, float LODBias) = 0;

        [[nodiscard]] virtual U64 getIdentifier() = 0;

        [[nodiscard]] VL_TEXTURE_FILTER getMinFilter() const { return m_MinFilter; }

        [[nodiscard]] VL_TEXTURE_FILTER getMagFilter() const { return m_MagFilter; }

        [[nodiscard]] VL_TEXTURE_WRAP getWrapU() const { return m_WrapU; }

        [[nodiscard]] VL_TEXTURE_WRAP getWrapV() const { return m_WrapV; }

        [[nodiscard]] VL_TEXTURE_WRAP getWrapW() const { return m_WrapW; }

        [[nodiscard]] VL_COMPARISON_FUNC getComparisonFunc() const { return m_ComparisonFunc; }

        [[nodiscard]] const float* getBorderColor() const { return m_BorderColor; }

        [[nodiscard]] float getMinLOD() const { return m_MinLOD; }

        [[nodiscard]] float getMaxLOD() const { return m_MaxLOD; }

        [[nodiscard]] float getLODBias() const { return m_LODBias; }

    protected:
        const Device& m_Device;

        VL_TEXTURE_FILTER m_MinFilter;
        VL_TEXTURE_FILTER m_MagFilter;
        VL_TEXTURE_WRAP m_WrapU;
        VL_TEXTURE_WRAP m_WrapV;
        VL_TEXTURE_WRAP m_WrapW;
        VL_COMPARISON_FUNC m_ComparisonFunc;
        float m_BorderColor[4] = {0.0f, 0.0f, 0.0f, 0.0f };
        float m_MinLOD;
        float m_MaxLOD;
        float m_LODBias;
    };

}