#pragma once

#include <VelyraCore/Context/ShaderModule.hpp>

namespace Velyra::Core {

    class GLShaderModule : public ShaderModule {
    public:
        explicit GLShaderModule(const ShaderModuleDesc& desc);

        explicit GLShaderModule(const ShaderModuleFileDesc& desc);

        ~GLShaderModule() override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:

        std::string loadShaderFromFile(const fs::path& filePath) const;

        void createShader(const std::string& code);

    private:
        GLuint m_ShaderModuleID = 0;
        Utils::LogPtr m_Logger;

    };
}