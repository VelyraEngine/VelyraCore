#pragma once

#include <VelyraCore/Context/StructuredBuffer.hpp>
#include "Internal/GLBuffer.hpp"

namespace Velyra::Core {

    class GLStructuredBuffer: public StructuredBuffer {
    public:
        GLStructuredBuffer(const StructuredBufferDesc& desc, const Device& device);

        ~GLStructuredBuffer() override = default;

        void bind() override;

        void bindShaderResource(U32 slot) override;

        void setData(U64 offset, const void* data, Size size) override;

        void copyFrom(const View<StructuredBuffer>& other) override;

        [[nodiscard]] std::vector<std::byte> getData() const override;

        [[nodiscard]] U64 getIdentifier() override;
    private:
        const Utils::LogPtr m_Logger = Utils::getLogger(VL_LOGGER_OGL);
        GLBuffer m_Buffer;
    };

}