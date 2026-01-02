#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    struct VL_API IndexBufferDesc {
        const void* data        = nullptr;
        Size count              = 0;
        VL_BUFFER_USAGE usage   = VL_BUFFER_USAGE_DEFAULT;
        VL_TYPE dataType        = VL_UINT32;
    };

    class VL_API IndexBuffer {
    public:
        explicit IndexBuffer(const IndexBufferDesc& desc)
            : m_Count(desc.count), m_Usage(desc.usage), m_DataType(desc.dataType) {}

        virtual ~IndexBuffer() = default;

        virtual void bind() = 0;

        virtual void setData(U64 offset, const void* data, U64 count) = 0;

        virtual void copyFrom(const SP<IndexBuffer>& other) = 0;

        [[nodiscard]] virtual std::vector<UByte> getData() const = 0;

        [[nodiscard]] U64 getIndexCount() const {
            return m_Count;
        }

    protected:
        const Size m_Count = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        const VL_TYPE m_DataType;

    };
}