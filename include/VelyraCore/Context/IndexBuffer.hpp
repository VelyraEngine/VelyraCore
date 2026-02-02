#pragma once

#include <VelyraUtils/Memory/View.hpp>
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

        /**
         * @brief Binds the index buffer to the pipeline (Only used for stateful APIs like OpenGL).
         *        Since the OpenGL implementation is based on Direct State Access, this function should not be
         *        used but can be handy for certain debugging scenarios (for example RenderDoc captures).
         */
        virtual void bind() = 0;

        /**
         * @brief Updates the index buffer data starting from the given offset.
         * @param offset Number of indices to offset (not in bytes).
         * @param data Pointer to the new index data.
         * @param count Number of indices to update.
         */
        virtual void setData(U64 offset, const void* data, U64 count) = 0;

        /**
         * @brief Copies the contents from another index buffer into this one.
         * @param other The source index buffer to copy from.
         */
        virtual void copyFrom(const View<IndexBuffer>& other) = 0;

        /**
         * @brief Retrieves the index buffer data by reading the data from the GPU.
         * @return A Byte array containing the index buffer data.
         */
        [[nodiscard]] virtual std::vector<std::byte> getData() const = 0;

        /**
         * @brief Returns the unique identifier of the index buffer (e.g., OpenGL buffer ID).
         * @return
         */
        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        /**
         * @brief Returns the number of indices in the index buffer.
         * @return
         */
        [[nodiscard]] U64 getCount() const {
            return m_Count;
        }

        /**
         * @brief Returns the size of the index buffer in bytes.
         * @return
         */
        [[nodiscard]] Size getSize() const {
            return m_Count * Utils::getTypeSize(m_DataType);
        }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const {
            return m_Usage;
        }

        [[nodiscard]] VL_TYPE getDataType() const {
            return m_DataType;
        }

    protected:
        const Size m_Count = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        const VL_TYPE m_DataType;

    };
}