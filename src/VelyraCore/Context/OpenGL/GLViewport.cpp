#include "GLViewport.hpp"

namespace Velyra::Core {

    GLViewport::GLViewport(const ViewportDesc &desc):
    Viewport(),
    m_Width(desc.width),
    m_Height(desc.height),
    m_XPos(desc.xPosition),
    m_YPos(desc.yPosition),
    m_MinDepth(desc.minDepth),
    m_MaxDepth(desc.maxDepth) {
    }

    void GLViewport::bind() {
        glViewport(m_XPos, m_YPos, m_Width, m_Height);
        glDepthRange(m_MinDepth, m_MaxDepth);
    }

    void GLViewport::resize(const U32 width, const U32 height) {
        m_Width = width;
        m_Height = height;
    }

    void GLViewport::setPosition(const U32 x, const U32 y) {
        m_XPos = x;
        m_YPos = y;
    }

    void GLViewport::setDepthRange(const float minDepth, const float maxDepth) {
        m_MinDepth = minDepth;
        m_MaxDepth = maxDepth;
    }

    U32 GLViewport::getWidth() const {
        return m_Width;
    }

    U32 GLViewport::getHeight() const {
        return m_Height;
    }

    U32 GLViewport::getXPos() const {
        return m_XPos;
    }

    U32 GLViewport::getYPos() const {
        return m_YPos;
    }

    float GLViewport::getMinDepth() const {
        return m_MinDepth;
    }

    float GLViewport::getMaxDepth() const {
        return m_MaxDepth;
    }

}
