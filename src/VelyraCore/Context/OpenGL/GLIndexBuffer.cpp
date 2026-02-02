#include "../../Pch.hpp"

#include "GLIndexBuffer.hpp"
#include "../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLIndexBuffer::GLIndexBuffer(const IndexBufferDesc &desc):
    IndexBuffer(desc),
    m_Buffer(GL_ELEMENT_ARRAY_BUFFER, desc.count * Utils::getTypeSize(desc.dataType), desc.data, desc.usage){

    }

    void GLIndexBuffer::bind() {
        m_Buffer.bind();
    }

    void GLIndexBuffer::setData(const U64 offset, const void *data, const U64 count) {
        const Size copySize = count * Utils::getTypeSize(m_DataType);
        const Size byteOffset = offset * Utils::getTypeSize(m_DataType);
        m_Buffer.setData(byteOffset, data, copySize);
    }

    void GLIndexBuffer::copyFrom(const View<IndexBuffer> &other) {
        const auto glBuffer = dynamic_cast<GLIndexBuffer*>(other.get());
        if (!glBuffer) {
            const Utils::LogPtr logger = Utils::getLogger(VL_LOGGER_OGL);
            SPDLOG_LOGGER_ERROR(logger, "Failed to copy contents of IndexBuffer object {} to IndexBuffer object {}",
                other->getIdentifier(), m_Buffer.getBufferID());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    std::vector<std::byte> GLIndexBuffer::getData() const {
        return m_Buffer.getData();
    }

    U64 GLIndexBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

}
