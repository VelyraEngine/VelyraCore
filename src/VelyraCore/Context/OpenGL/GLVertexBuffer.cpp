#include "../../Pch.hpp"

#include "GLVertexBuffer.hpp"
#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLVertexBuffer::GLVertexBuffer(const VertexBufferDesc &desc):
    VertexBuffer(desc),
    m_Buffer(GL_ARRAY_BUFFER, desc.layout->getStride() * desc.count, desc.data, desc.usage){

    }

    void GLVertexBuffer::bind() {
        m_Buffer.bind();
    }

    void GLVertexBuffer::setData(const U64 offset, const void *data, const U64 count) {
        const Size copySize = count * m_Layout->getStride();
        const Size byteOffset = offset * m_Layout->getStride();
        m_Buffer.setData(byteOffset, data, copySize);
    }

    void GLVertexBuffer::copyFrom(const SP<VertexBuffer> &other) {
        const auto glBuffer = dynamic_cast<GLVertexBuffer*>(other.get());
        if (!glBuffer) {
            const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
            SPDLOG_LOGGER_ERROR(logger, "Failed to copy contents of VertexBuffer object {} to VertexBuffer object {}",
                other->getIdentifier(), m_Buffer.getBufferID());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    std::vector<std::byte> GLVertexBuffer::getData() const {
        return m_Buffer.getData();
    }

    U64 GLVertexBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

}