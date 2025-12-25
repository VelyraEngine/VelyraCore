#pragma once

#include <VelyraCore/Context/Shader.hpp>

namespace Velyra::Core {

    class GLShader : public Shader {
    public:
        explicit GLShader(const ShaderDesc& desc);

        ~GLShader() override;

        void bind() override;

        [[nodiscard]] U64 getIdentifier() const override;

    private:

        void createProgram(const ShaderDesc& desc);

    private:
        const Utils::LogPtr m_Logger;
        GLuint m_ShaderID = 0;
    };
}