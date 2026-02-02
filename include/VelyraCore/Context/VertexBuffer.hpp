#pragma once

#include <VelyraUtils/Memory/View.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>

namespace Velyra::Core {

    struct VL_API VertexBufferDesc {
        const void* data        = nullptr;
        Size count              = 0;        // Amount of vertices, so the size inf bytes is count * layout->getStride()
        VL_BUFFER_USAGE usage   = VL_BUFFER_USAGE_DEFAULT;
        View<VertexLayout> layout = nullptr;
    };

    class VL_API VertexBuffer {
    public:
        explicit VertexBuffer(const VertexBufferDesc& desc)
            : m_Count(desc.count), m_Usage(desc.usage), m_Layout(desc.layout) {}

        virtual ~VertexBuffer() = default;

        /**
         * @brief Binds the vertex buffer to the pipeline (Only used for stateful APIs like OpenGL).
         *        Since the OpenGL implementation is based on Direct State Access, this function should not be
         *        used but can be handy for certain debugging scenarios (for example RenderDoc captures).
         */
        virtual void bind() = 0;

        /**
         * @brief Updates the vertex buffer data starting from the given offset.
         * @param offset Number of vertices to offset (not in bytes).
         * @param data Pointer to the new vertex data.
         * @param count Number of vertices to update.
         */
        virtual void setData(U64 offset, const void* data, U64 count) = 0;

        /**
         * @brief Copies the contents from another vertex buffer into this one.
         * @param other The source vertex buffer to copy from.
         */
        virtual void copyFrom(const View<VertexBuffer>& other) = 0;

        /**
         * @brief Retrieves the vertex buffer data by reading the data from the GPU.
         * @return A Byte array containing the vertex buffer data.
         */
        [[nodiscard]] virtual std::vector<std::byte> getData() const = 0;

        /**
         * @brief Returns the unique identifier of the vertex buffer (e.g., OpenGL buffer ID).
         * @return
         */
        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        /**
         * @brief Returns the size of the vertex buffer in bytes.
         * @return
         */
        [[nodiscard]] U64 getSize() const {
            return m_Count * m_Layout->getStride();
        }

        /**
         * @brief Returns the amount of vertices stored in the vertex buffer.
         * @return
         */
        [[nodiscard]] Size getCount() const {
            return m_Count;
        }

        /**
         * @brief Returns the layout of the vertex data.
         * @return
         */
        [[nodiscard]] View<VertexLayout> getLayout() const {
            return m_Layout;
        }

        /**
         * @brief Returns the usage pattern of the vertex buffer.
         * @return
         */
        [[nodiscard]] VL_BUFFER_USAGE getUsage() const {
            return m_Usage;
        }

    protected:
        const Size m_Count = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        const View<VertexLayout> m_Layout;
    };
}