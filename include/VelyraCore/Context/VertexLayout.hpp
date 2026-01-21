#pragma once

#include <VelyraCore/Context/Device.hpp>

namespace Velyra::Core {

    struct VL_API VertexAttribute {
        std::string name;
        VL_SCALAR_TYPE type;

        VertexAttribute() = default;
        VertexAttribute(const std::string& name_, const VL_SCALAR_TYPE type_)
            : name(name_), type(type_) {}
    };

    class VL_API VertexLayout {
    public:
        explicit VertexLayout(const Device& device);

        ~VertexLayout() = default;

        void bind();

        void addAttribute(const std::string& name, VL_SCALAR_TYPE dataType);

        const std::vector<VertexAttribute>& getAttributes() const {
            return m_Attributes;
        }

        U32 getStride() const {
            return m_Stride;
        }

        Size getAttributeCount() const {
            return m_Attributes.size();
        }

        bool hasAttribute(const std::string& name) const;

    protected:
        const Device& m_Device;
        const Utils::LogPtr m_Logger;
        std::vector<VertexAttribute> m_Attributes;
        U32 m_Stride = 0;
    };

}