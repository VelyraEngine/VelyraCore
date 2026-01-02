#include "../../../Pch.hpp"

#include "GLBuffer.hpp"

#include "../../../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    GLBuffer::GLBuffer(const GLenum target, const Size size, const void *data, const VL_BUFFER_USAGE usage):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_Size(size),
    m_Usage(usage),
    m_Target(target) {
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, size, data, getGLBufferUsage(usage));

        SPDLOG_LOGGER_TRACE(m_Logger, "Buffer object {} created! (size: {}, usage: {})",
            m_BufferID, size, usage);
    }

    GLBuffer::GLBuffer(const std::string &name, const GLenum target, const Size size, const void *data, const VL_BUFFER_USAGE usage):
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_Name(name),
    m_Size(size),
    m_Usage(usage),
    m_Target(target){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, size, data, getGLBufferUsage(usage));

        SPDLOG_LOGGER_TRACE(m_Logger, "Buffer object {} created! (size: {}, usage: {}, name: {})",
            m_BufferID, size, usage, m_Name);
    }

    GLBuffer::~GLBuffer() {
        glDeleteBuffers(1, &m_BufferID);

        SPDLOG_LOGGER_TRACE(m_Logger, "Buffer object {} deleted! (name: {})", m_BufferID, m_Name);
    }

    void GLBuffer::bind() const {
        if (m_BufferID == 0) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Buffer {} not created!", m_BufferID);
            return;
        }
        glBindBuffer(m_Target, m_BufferID);
    }

    void GLBuffer::bindShaderResource(const U32 slot) const {
        if (m_BufferID == 0) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Buffer {} not created!", m_BufferID);
            return;
        }
        glBindBufferBase(m_Target, slot, m_BufferID);
    }

    void GLBuffer::setData(U64 offset, const void *data, U64 size) {
        if (m_BufferID == 0) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Buffer {} not created!", m_BufferID);
            return;
        }
        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Update on buffer object {} requested, which was created with {} usage flag!", m_BufferID, m_Usage);
            return;
        }
        if (offset >= m_Size) {
            SPDLOG_LOGGER_WARN(m_Logger, "Offset {} exceeds the current buffer size {}. No data will be written!", offset, m_Size);
            return;
        }

        // 1. Check Sizes
        if (offset + size > m_Size) {
            SPDLOG_LOGGER_WARN(m_Logger, "Data size {} with offset {} exceeds buffer size {}. Data will be partially written!", size, offset, m_Size);
            size = m_Size - offset;
        }

        // 2. Map the buffer
        void* pBuffer = glMapNamedBufferRange(m_BufferID, offset, size, GL_MAP_WRITE_BIT);
        if (!pBuffer) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to map buffer object {}", m_BufferID);
            return;
        }

        // 3. Copy data
        memcpy(pBuffer, data, size);
        if (glUnmapNamedBuffer(m_BufferID) == GL_FALSE) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to unmap buffer object {}", m_BufferID);
        }

        SPDLOG_LOGGER_TRACE(m_Logger, "Set {} bytes of data to buffer object {} at offset {}",
            size, m_BufferID, offset);
    }

    void GLBuffer::copyFrom(const GLBuffer &other) {
        if (m_Usage == VL_BUFFER_USAGE_STATIC) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Update on buffer object {} requested, which was created with {} usage flag!", m_BufferID, m_Usage);
            return;
        }
        if (m_Size < other.m_Size) {
            SPDLOG_LOGGER_WARN(m_Logger, "Source buffer size {} exceeds destination buffer size {}. Data will be partially copied!", other.m_Size, m_Size);
        }
        const Size copySize = std::min(m_Size, other.m_Size);
        glCopyNamedBufferSubData(other.m_BufferID, m_BufferID, 0, 0, copySize);

        SPDLOG_LOGGER_TRACE(m_Logger, "Copied {} bytes from buffer object {} to buffer object {}",
            copySize, other.m_BufferID, m_BufferID);
    }

    std::vector<std::byte> GLBuffer::getData() const {
        std::vector<std::byte> buffer(m_Size);
        const void* pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to map buffer object {}", m_BufferID);
            return std::move(buffer);
        }
        memcpy(buffer.data(), pBuffer, m_Size);
        if (glUnmapNamedBuffer(m_BufferID) == GL_FALSE) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Failed to unmap buffer object {}", m_BufferID);
        }
        return std::move(buffer);
    }
}
