#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    struct VL_API StructuredBufferDesc {
        const void* data = nullptr;
        Size size = 0; // Size in bytes
        VL_BUFFER_USAGE usage = VL_BUFFER_USAGE_DEFAULT;
        VL_SHADER_TYPE shaderStage = VL_SHADER_TYPE_MAX_VALUE;
    };

    class VL_API StructuredBuffer {
    public:
        StructuredBuffer(const StructuredBufferDesc& desc, const Device& device):
        m_Device(device),
        m_Size(desc.size),
        m_Usage(desc.usage),
        m_ShaderStage(desc.shaderStage){}

        virtual ~StructuredBuffer() = default;

        virtual void bind() = 0;

        virtual void bindShaderResource(U32 slot) = 0;

        virtual void setData(U64 offset, const void* data, Size size) = 0;

        virtual void copyFrom(const View<StructuredBuffer>& other) = 0;

        [[nodiscard]] virtual std::vector<std::byte> getData() const = 0;

        [[nodiscard]] virtual U64 getIdentifier() = 0;

        [[nodiscard]] Size getSize() const { return m_Size; }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const { return m_Usage; }

        [[nodiscard]] VL_SHADER_TYPE getShaderStage() const { return m_ShaderStage; }

    protected:
        const Device& m_Device;
        const Size m_Size = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;
        const VL_SHADER_TYPE m_ShaderStage = VL_SHADER_TYPE_MAX_VALUE;
    };


}