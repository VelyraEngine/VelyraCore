#pragma once

#include <VelyraUtils/Memory/View.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>

namespace Velyra::Core {

    struct VL_API ShaderDesc {
        View<ShaderModule> vertexShader;
        View<ShaderModule> fragmentShader;
    };

    class VL_API Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;
    };
}