#include "../../Pch.hpp"

#include "GLMeshBinding.hpp"
#include "../../Logging/LoggerNames.hpp"
#include "../../Logging/GLLogging.hpp"
#include "Internal/GLTranslations.hpp"

namespace Velyra::Core {

    GLArrayMeshBinding::GLArrayMeshBinding(const MeshBindingDesc &desc):
    MeshBinding(desc),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)),
    m_ArrayID(0),
    m_GlDrawMode(getGLDrawMode(desc.drawMode)),
    m_VertexBuffer(desc.vertexBuffer){

        GLuint offset = 0;
        GLuint attribPtr = 0;
        const std::vector<VertexAttribute>& attributes = desc.vertexBuffer->getLayout()->getAttributes();

        glCreateVertexArrays(1, &m_ArrayID);
        for (const auto& attribute: attributes) {
            const VL_TYPE baseType = Utils::getBaseType(attribute.type);
            const GLenum glDataType = getGLDataType(baseType);
            const GLint elementCount = static_cast<GLint>(Utils::getComponentCount(attribute.type));

            glEnableVertexArrayAttrib(m_ArrayID, attribPtr);
            glVertexArrayAttribBinding(m_ArrayID, attribPtr, 0);
            glVertexArrayAttribFormat(m_ArrayID, attribPtr, elementCount, glDataType, GL_FALSE, offset);

            attribPtr++;
            offset += Utils::getTypeSize(attribute.type);
        }

        // For OpenGL vertex buffers, the identifier is guaranteed to be GLuint, so we can static cast here.
        const auto vertexBufferID = static_cast<GLuint>(desc.vertexBuffer->getIdentifier());
        glVertexArrayVertexBuffer(m_ArrayID, 0, vertexBufferID, 0, static_cast<GLsizei>(offset));

        SPDLOG_LOGGER_TRACE(m_Logger, "Created GLMeshBinding with ID {} (Attributes: {}, Offset: {})", m_ArrayID, attribPtr, offset);
    }

    GLArrayMeshBinding::~GLArrayMeshBinding() {
        glDeleteVertexArrays(1, &m_ArrayID);
    }

    void GLArrayMeshBinding::draw() const {
        /*
         * From experience, RenderDoc complains about incorrect API usage if we don't bind the vertex buffer explicitly,
         * although with DSA it shouldn't be necessary (and works fine if we don't bind it). So we bind it only in debug mode.
         */
#if defined(VL_DEBUG)
        m_VertexBuffer->bind();
#endif
        glBindVertexArray(m_ArrayID);

        // Then draw call
        const auto vertexCount = static_cast<GLsizei>(m_VertexBuffer->getCount());
        SPDLOG_LOGGER_TRACE(m_Logger, "Drawing MeshBinding: {} with {} vertices", m_ArrayID, vertexCount);
        glDrawArrays(m_GlDrawMode, 0, vertexCount);

        // Unbind VAO to prevent state pollution
        glBindVertexArray(0);

        VL_CORE_OPENGL_LOG_ERRORS();
    }

    void GLArrayMeshBinding::drawInstanced(const U64 instanceCount) const {
#if defined(VL_DEBUG)
        m_VertexBuffer->bind();
#endif
        glBindVertexArray(m_ArrayID);

        const auto vertexCount = static_cast<GLsizei>(m_VertexBuffer->getCount());
        SPDLOG_LOGGER_TRACE(m_Logger, "Drawing MeshBinding: {} with {} vertices and {} instances", m_ArrayID, vertexCount, instanceCount);
        glDrawArraysInstanced(m_GlDrawMode, 0, vertexCount, static_cast<GLsizei>(instanceCount));

        // Unbind VAO to prevent state pollution
        glBindVertexArray(0);

        VL_CORE_OPENGL_LOG_ERRORS();
    }

    void GLArrayMeshBinding::setDrawMode(const VL_DRAW_MODE drawMode) {
        m_DrawMode = drawMode;
        m_GlDrawMode = getGLDrawMode(drawMode);
    }

    /////////////////// GLIndexedMeshBinding ///////////////////

    GLIndexedMeshBinding::GLIndexedMeshBinding(const MeshBindingDesc &desc):
    GLArrayMeshBinding(desc),
    m_IndexBuffer(desc.indexBuffer),
    m_IndexType(getGLDataType(desc.indexBuffer->getDataType())){
        // We only need to bind the index buffer to the vertex array object.
        const auto indexBufferID = static_cast<GLuint>(desc.indexBuffer->getIdentifier());
        glVertexArrayElementBuffer(m_ArrayID, indexBufferID);
    }

    void GLIndexedMeshBinding::draw() const {
        // Same story here regarding RenderDoc and debug mode.
        glBindVertexArray(m_ArrayID);

#if defined(VL_DEBUG)
        // Bind buffers AFTER VAO to avoid corrupting other VAOs' state
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
#endif

        const auto indexCount = static_cast<GLsizei>(m_IndexBuffer->getCount());
        SPDLOG_LOGGER_TRACE(m_Logger, "Drawing Indexed MeshBinding: {} with {} indices", m_ArrayID, indexCount);
        glDrawElements(m_GlDrawMode, indexCount, m_IndexType, nullptr);

        // Unbind VAO to prevent state pollution
        glBindVertexArray(0);

        VL_CORE_OPENGL_LOG_ERRORS();
    }

    void GLIndexedMeshBinding::drawInstanced(const U64 instanceCount) const {
        glBindVertexArray(m_ArrayID);

#if defined(VL_DEBUG)
        // Bind buffers AFTER VAO to avoid corrupting other VAOs' state
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
#endif

        const auto indexCount = static_cast<GLsizei>(m_IndexBuffer->getCount());
        SPDLOG_LOGGER_TRACE(m_Logger, "Drawing Indexed MeshBinding: {} with {} indices and {} instances", m_ArrayID, indexCount, instanceCount);
        glDrawElementsInstanced(m_GlDrawMode, indexCount, m_IndexType, nullptr, static_cast<GLsizei>(instanceCount));

        // Unbind VAO to prevent state pollution
        glBindVertexArray(0);

        VL_CORE_OPENGL_LOG_ERRORS();
    }




}