#include "../../Pch.hpp"

#include "GLConstantBuffer.hpp"

namespace Velyra::Core {

    GLConstantBuffer::GLConstantBuffer(const ConstantBufferDesc &desc, const Device& device):
    ConstantBuffer(desc, device),
    m_Buffer(desc.name, GL_UNIFORM_BUFFER, desc.size, desc.data, desc.usage){

    }

    void GLConstantBuffer::bind() {
        m_Buffer.bind();
    }

    void GLConstantBuffer::bindShaderResource(const U32 slot) {
        if (slot > m_Device.getMaxConstantBufferSlots()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "ConstantBuffer {}: supplied slot {} exceeds the maximum number of constant buffer slots {}",
                m_Buffer.getName(), slot, m_Device.getMaxConstantBufferSlots());
            return;
        }
        m_Buffer.bindShaderResource(slot);
    }

    void GLConstantBuffer::setData(const U64 offset, const void *data, const U64 count) {
        m_Buffer.setData(offset, data, count);
    }

    void GLConstantBuffer::copyFrom(const View<ConstantBuffer> &other) {
        const auto glBuffer = dynamic_cast<GLConstantBuffer*>(other.get());
        if (!glBuffer) {
            const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
            SPDLOG_LOGGER_ERROR(logger, "Failed to copy contents of ConstantBuffer object {} to ConstantBuffer object {}",
                other->getIdentifier(), m_Buffer.getBufferID());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    std::vector<std::byte> GLConstantBuffer::getData() const {
        return m_Buffer.getData();
    }

    U64 GLConstantBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

}
