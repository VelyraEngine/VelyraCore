#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <filesystem>

namespace Velyra::Core {

    namespace fs = std::filesystem;

    struct VL_API ShaderModuleDesc {
        std::string code;   // shader code
        VL_SHADER_TYPE shaderType   = VL_SHADER_TYPE_MAX_VALUE;
        std::string entryPoint      = "main";
    };

    struct VL_API ShaderModuleFileDesc {
        fs::path filePath;
        VL_SHADER_TYPE shaderType   = VL_SHADER_TYPE_MAX_VALUE;
        std::string entryPoint      = "main";
    };

    class VL_API ShaderModule {
    public:
        explicit ShaderModule(const ShaderModuleDesc& desc):
        m_ShaderType(desc.shaderType),
        m_EntryPoint(desc.entryPoint){

        }

        explicit ShaderModule(const ShaderModuleFileDesc& desc):
        m_ShaderType(desc.shaderType),
        m_EntryPoint(desc.entryPoint){

        }

        virtual ~ShaderModule() = default;

        [[nodiscard]] virtual U64 getIdentifier() const = 0;

        [[nodiscard]] VL_SHADER_TYPE getShaderModuleType() const {
            return m_ShaderType;
        }

        [[nodiscard]] const std::string& getEntryPoint() const {
            return m_EntryPoint;
        }

    protected:
        const VL_SHADER_TYPE m_ShaderType;
        const std::string m_EntryPoint;
    };
}

