#pragma once

#include <VelyraCore/Context/IndexBuffer.hpp>
#include "Internal/GLBuffer.hpp"

namespace Velyra::Core {

    class GLIndexBuffer : public IndexBuffer {
    public:
        explicit GLIndexBuffer(const IndexBufferDesc& desc);

        ~GLIndexBuffer() override = default;

        void bind() override;

        void setData(U64 offset, const void *data, U64 count) override;

        void copyFrom(const View<IndexBuffer> &other) override;

        [[nodiscard]] std::vector<std::byte> getData() const override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:
        GLBuffer m_Buffer;
    };
}