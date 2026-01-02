#include "../Pch.hpp"

#include <VelyraCore/Context/VertexLayout.hpp>
#include "../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    VertexLayout::VertexLayout(const Device& device) :
    m_Device(device),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){}


    void VertexLayout::bind() {
        // Default implementation does nothing
    }

    void VertexLayout::addAttribute(const std::string &name, VL_SCALAR_TYPE dataType) {
        if (hasAttribute(name)) {
            SPDLOG_LOGGER_WARN(m_Logger, "Vertex attribute with name {} already exists", name);
            return;
        }
        m_Attributes.emplace_back(name, dataType);
        m_Stride += Utils::getTypeSize(dataType);
    }

    bool VertexLayout::hasAttribute(const std::string &name) const {
        return std::ranges::any_of(m_Attributes,
                                   [&name](const VertexAttribute &attribute) {
                                       return attribute.name == name;
                                   });
    }

}