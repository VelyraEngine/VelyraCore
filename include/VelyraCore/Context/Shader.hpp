#pragma once

#include <VelyraCore/Context/ShaderModule.hpp>

namespace Velyra::Core {

    struct VL_API ShaderDesc {
        SP<ShaderModule> vertexShader;
        SP<ShaderModule> fragmentShader;
    };

    class VL_API Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;
    };
}