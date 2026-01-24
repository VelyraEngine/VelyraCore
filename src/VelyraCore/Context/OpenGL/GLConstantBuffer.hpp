#pragma once

#include <VelyraCore/Context/ConstantBuffer.hpp>
#include "Internal/GLBuffer.hpp"
#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    class GLConstantBuffer : public ConstantBuffer {
    public:
        explicit GLConstantBuffer(const ConstantBufferDesc& desc, const Device& device);

        ~GLConstantBuffer() override = default;

        void bind() override;

        void bindShaderResource(U32 slot) override;

        void setData(U64 offset, const void *data, U64 count) override;

        void copyFrom(const SP<ConstantBuffer> &other) override;

        [[nodiscard]] std::vector<std::byte> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        const Utils::LogPtr m_Logger = Utils::getLogger(VL_LOGGER_OGL);
        GLBuffer m_Buffer;
    };
}