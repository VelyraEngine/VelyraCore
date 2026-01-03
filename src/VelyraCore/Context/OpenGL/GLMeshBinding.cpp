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
            const U32 elementCount = Utils::getComponentCount(attribute.type);

            glEnableVertexArrayAttrib(m_ArrayID, attribPtr);
            glVertexArrayAttribBinding(m_ArrayID, attribPtr, 0);
            glVertexArrayAttribFormat(m_ArrayID, attribPtr, elementCount, glDataType, GL_FALSE, offset);

            attribPtr++;
            offset += Utils::getTypeSize(attribute.type);
        }

        // For OpenGL vertex buffers, the identifier is guaranteed to be GLuint, so we can static cast here.
        const auto vertexBufferID = static_cast<GLuint>(desc.vertexBuffer->getIdentifier());
        glVertexArrayVertexBuffer(m_ArrayID, 0, vertexBufferID, 0, offset);

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
        const GLsizei vertexCount = static_cast<GLsizei>(m_VertexBuffer->getCount());
        glDrawArrays(m_GlDrawMode, 0, vertexCount);

        VL_CORE_OPENGL_LOG_ERRORS();
    }

    void GLArrayMeshBinding::drawInstanced(const U64 instanceCount) const {
#if defined(VL_DEBUG)
        m_VertexBuffer->bind();
#endif
        glBindVertexArray(m_ArrayID);

        const GLsizei vertexCount = static_cast<GLsizei>(m_VertexBuffer->getCount());
        glDrawArraysInstanced(m_GlDrawMode, 0, vertexCount, static_cast<GLsizei>(instanceCount));

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
#if defined(VL_DEBUG)
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
#endif
        glBindVertexArray(m_ArrayID);

        const GLsizei indexCount = static_cast<GLsizei>(m_IndexBuffer->getCount());
        glDrawElements(m_GlDrawMode, indexCount, m_IndexType, nullptr);

        VL_CORE_OPENGL_LOG_ERRORS();
    }

    void GLIndexedMeshBinding::drawInstanced(const U64 instanceCount) const {
#if defined(VL_DEBUG)
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
#endif
        glBindVertexArray(m_ArrayID);

        const GLsizei indexCount = static_cast<GLsizei>(m_IndexBuffer->getCount());
        glDrawElementsInstanced(m_GlDrawMode, indexCount, m_IndexType, nullptr, static_cast<GLsizei>(instanceCount));

        VL_CORE_OPENGL_LOG_ERRORS();
    }




}