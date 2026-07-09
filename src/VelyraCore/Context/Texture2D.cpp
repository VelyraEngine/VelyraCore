#include "../Pch.hpp"

#include <VelyraCore/Context/Texture2D.hpp>

namespace Velyra::Core {

    Texture2D::Texture2D(const Texture2DDesc& desc, const Device& device):
        m_Device(device),
        m_Width(desc.width),
        m_Height(desc.height),
        m_Format(desc.format),
        m_Usage(desc.usage) {
    }

    Texture2D::Texture2D(const Texture2DImageDesc& desc, const Device& device):
        m_Device(device),
        m_Width(desc.image->getWidth()),
        m_Height(desc.image->getHeight()),
        m_Format(getTextureFormat(desc.image)),
        m_Usage(desc.usage) {
    }

}
