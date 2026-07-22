#include "../../Pch.hpp"

#include "GLStructuredBuffer.hpp"
#include "../../Logging/LoggerNames.hpp"
#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    GLStructuredBuffer::GLStructuredBuffer(const StructuredBufferDesc &desc, const Device& device):
    StructuredBuffer(desc, device),
    m_Buffer(GL_SHADER_STORAGE_BUFFER, desc.size, desc.data, desc.usage){

    }

    void GLStructuredBuffer::bind() {
        m_Buffer.bind();
    }

    void GLStructuredBuffer::bindShaderResource(const U32 slot) {
        if (slot > m_Device.getMaxStructuredBufferSlots()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "StructuredBuffer {}: supplied slot {} exceeds the maximum number of structured buffer slots {}",
                m_Buffer.getName(), slot, m_Device.getMaxStructuredBufferSlots());
            return;
        }

        m_Buffer.bindShaderResource(slot);
    }

    void GLStructuredBuffer::setData(U64 offset, const void *data, Size size) {
        m_Buffer.setData(offset, data, size);
    }

    void GLStructuredBuffer::copyFrom(const View<StructuredBuffer> &other) {
        const auto glBuffer = dynamic_cast<GLStructuredBuffer*>(other.get());
        if (!glBuffer) {
            const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
            SPDLOG_LOGGER_ERROR(logger, "Failed to copy contents of StructuredBuffer object {} to StructuredBuffer object {}",
                other->getIdentifier(), m_Buffer.getBufferID());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    std::vector<std::byte> GLStructuredBuffer::getData() const {
        return m_Buffer.getData();
    }

    U64 GLStructuredBuffer::getIdentifier() {
        return m_Buffer.getBufferID();
    }

}
