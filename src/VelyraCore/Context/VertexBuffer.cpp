#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>

namespace Velyra::Core {

    VertexBuffer::VertexBuffer(const VertexBufferDesc &desc):
    m_Count(desc.count),
    m_Usage(desc.usage), m_Layout(desc.layout) {

    }

    U64 VertexBuffer::getSize() const {
        return m_Count * m_Layout->getStride();
    }
}
