#pragma once

#include <VelyraCore/Context/Sampler.hpp>

namespace Velyra::Core {

    class GLSampler : public Sampler {
    public:
        explicit GLSampler(const Device& device, const SamplerDesc& desc);

        ~GLSampler() override;

        void bindShaderResource(U32 slot) override;

        void setFilter(VL_TEXTURE_FILTER minFilter, VL_TEXTURE_FILTER magFilter) override;

        void setWrap(VL_TEXTURE_WRAP wrapU, VL_TEXTURE_WRAP wrapV, VL_TEXTURE_WRAP wrapW) override;

        void setComparisonFunc(VL_COMPARISON_FUNC comparisonFunc) override;

        void setBorderColor(float r, float g, float b, float a = 1.0f) override;

        void setLOD(float minLOD, float maxLOD, float LODBias) override;

        [[nodiscard]] U64 getIdentifier() override;

    private:
        const Utils::LogPtr m_Logger;
        U32 m_SamplerID;
    };

}