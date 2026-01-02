#pragma once

#include <VelyraCore/Context/VertexBuffer.hpp>
#include "Internal/GLBuffer.hpp"

namespace Velyra::Core {

    class GLVertexBuffer : public VertexBuffer {
    public:
        explicit GLVertexBuffer(const VertexBufferDesc& desc);

        ~GLVertexBuffer() override = default;

        void bind() override;

        void setData(U64 offset, const void *data, U64 count) override;

        void copyFrom(const SP<VertexBuffer> &other) override;

        [[nodiscard]] std::vector<UByte> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        GLBuffer m_Buffer;
    };
}