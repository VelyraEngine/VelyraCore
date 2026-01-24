#pragma once

#include "SandBoxInclude.hpp"

namespace Velyra::SandBox {

    namespace fs = std::filesystem;

    struct Vertex{
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord;

        static SP<Core::VertexLayout> getVertexLayout(const UP<Core::Context> &context);
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<U32> indices;
    };

    struct ShaderProgram {
        SP<Core::ShaderModule> vertexShader;
        SP<Core::ShaderModule> fragmentShader;
        SP<Core::Shader> shaderProgram;
    };

    ShaderProgram loadShader(const UP<Core::Context> &context, const std::string &vertexShaderName, const std::string &fragmentShaderName);
}