#pragma once

#include <VelyraUtils/Memory/View.hpp>
#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API ConstantBufferDesc {
        std::string name;
        Size size                     = 0;
        const void* data              = nullptr;
        VL_BUFFER_USAGE usage         = VL_BUFFER_USAGE_DEFAULT;
        VL_SHADER_TYPE shaderStage    = VL_SHADER_TYPE_MAX_VALUE;
    };

    class VL_API ConstantBuffer{
    public:
        ConstantBuffer(const ConstantBufferDesc& desc, const Device& device)
            : m_Device(device), m_Size(desc.size), m_Usage(desc.usage),
            m_Name(desc.name), m_ShaderStage(desc.shaderStage) {}

        virtual ~ConstantBuffer() = default;

        /**
         * @brief Binds the Constant Buffer to the pipeline (Only used for stateful APIs like OpenGL).
         *        Since the OpenGL implementation is based on Direct State Access, this function should not be
         *        used but can be handy for certain debugging scenarios (for example RenderDoc captures).
         */
        virtual void bind() = 0;

        /**
         * @brief Binds the Constant Buffer to the given shader resource slot.
         * @param slot The slot to bind the Constant Buffer to.
         */
        virtual void bindShaderResource(U32 slot) = 0;

        /**
         * @brief Updates the Constant Buffer data starting from the given offset.
         * @param offset Number of vertices to offset (not in bytes).
         * @param data Pointer to the new data.
         * @param count Number of vertices to update.
         */
        virtual void setData(U64 offset, const void* data, U64 count) = 0;

        /**
         * @brief Copies the contents from another Constant Buffer into this one.
         * @param other The source buffer to copy from.
         */
        virtual void copyFrom(const View<ConstantBuffer>& other) = 0;

        /**
         * @brief Retrieves the Constant Buffer data by reading the data from the GPU.
         * @return A Byte array containing the buffer data.
         */
        [[nodiscard]] virtual std::vector<std::byte> getData() const = 0;

        /**
         * @brief Returns the unique identifier of the Constant Buffer (e.g., OpenGL buffer ID).
         * @return
         */
        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        /**
         * @brief Returns the name of the Constant Buffer, this is the one that should be used in the shader.
         * @return
         */
        [[nodiscard]] const std::string& getName() const {
            return m_Name;
        }

        /**
         * @brief Returns the size of the Constant Buffer in bytes.
         * @return
         */
        [[nodiscard]] Size getSize() const {
            return m_Size;
        }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const {
            return m_Usage;
        }

        [[nodiscard]] VL_SHADER_TYPE getShaderStage() const {
            return m_ShaderStage;
        }

    protected:
        const Device& m_Device;
        const Size m_Size = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        const std::string m_Name;
        const VL_SHADER_TYPE m_ShaderStage = VL_SHADER_TYPE_MAX_VALUE;
    };
}