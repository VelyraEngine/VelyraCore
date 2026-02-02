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

        static View<Core::VertexLayout> getVertexLayout(const UP<Core::Context> &context);
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<U32> indices;
    };

    struct ShaderProgram {
        View<Core::ShaderModule> vertexShader;
        View<Core::ShaderModule> fragmentShader;
        View<Core::Shader> shaderProgram;
    };

    ShaderProgram loadShader(const UP<Core::Context> &context, const std::string &vertexShaderName, const std::string &fragmentShaderName);

    View<Core::MeshBinding> loadMesh(const Mesh& mesh, const UP<Core::Context> &context);

    Mesh createSphere(U32 rings, U32 sectors);

    Mesh createCube();
}