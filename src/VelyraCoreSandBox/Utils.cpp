#include "Utils.hpp"

#include "VelyraUtils/Math.hpp"

namespace Velyra::SandBox {

    View<Core::VertexLayout> Vertex::getVertexLayout(const UP<Core::Context> &context) {
        View<Core::VertexLayout> layout = context->createVertexLayout();
        layout->addAttribute("Position", VL_FLOAT32_3);
        layout->addAttribute("Color", VL_FLOAT32_3);
        layout->addAttribute("Normal", VL_FLOAT32_3);
        layout->addAttribute("Tangent", VL_FLOAT32_3);
        layout->addAttribute("TexCoord", VL_FLOAT32_2);
        return layout;
    }


    ShaderProgram loadShader(const UP<Core::Context> &context, const std::string &vertexShaderName,
        const std::string &fragmentShaderName) {
        fs::path library = fs::current_path() / "src" / "VelyraCoreSandBox" / "Shaders";
        std::string extension;
        switch (context->getType()) {
            case VL_API_OPENGL: {
                library /= "GLSL";
                extension = ".glsl";
                break;
            }
            default: {
                VL_THROW("Unsupported API {}", toString(context->getType()));
            }
        }

        ShaderProgram program;

        Core::ShaderModuleFileDesc vsDesc;
        vsDesc.shaderType = VL_SHADER_VERTEX;
        vsDesc.filePath = library / std::string(vertexShaderName + "_vs" + extension);
        vsDesc.entryPoint = "main";
        program.vertexShader = context->createShaderModule(vsDesc);

        Core::ShaderModuleFileDesc fsDesc;
        fsDesc.shaderType = VL_SHADER_FRAGMENT;
        fsDesc.filePath = library / std::string(fragmentShaderName + "_fs" + extension);
        fsDesc.entryPoint = "main";
        program.fragmentShader = context->createShaderModule(fsDesc);

        Core::ShaderDesc sDesc;
        sDesc.vertexShader = program.vertexShader;
        sDesc.fragmentShader = program.fragmentShader;
        program.shaderProgram = context->createShader(sDesc);

        return program;
    }

    View<Core::MeshBinding> loadMesh(const Mesh &mesh, const UP<Core::Context> &context) {
        const auto vertexLayout = Vertex::getVertexLayout(context);

        Core::VertexBufferDesc vboDesc;
        vboDesc.usage = VL_BUFFER_USAGE_DEFAULT;
        vboDesc.layout = vertexLayout;
        vboDesc.count = mesh.vertices.size();
        vboDesc.data = mesh.vertices.data();
        auto vbo = context->createVertexBuffer(vboDesc);

        Core::IndexBufferDesc iboDesc;
        iboDesc.dataType = VL_UINT32;
        iboDesc.usage = VL_BUFFER_USAGE_DEFAULT;
        iboDesc.count = mesh.indices.size();
        iboDesc.data = mesh.indices.data();
        auto ibo = context->createIndexBuffer(iboDesc);

        Core::MeshBindingDesc mbDesc;
        mbDesc.drawMode = VL_DRAW_TRIANGLES;
        mbDesc.vertexBuffer = vbo;
        mbDesc.indexBuffer = ibo;
        return context->createMeshBinding(mbDesc);
    }

    void calculateTangents(Mesh& mesh){
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            auto& v0 = mesh.vertices[mesh.indices[i + 0]];
            auto& v1 = mesh.vertices[mesh.indices[i + 1]];
            auto& v2 = mesh.vertices[mesh.indices[i + 2]];

            const auto edge1 = v1.position - v0.position;
            const auto edge2 = v2.position - v0.position;

            const auto deltaUV1 = v1.texCoord - v0.texCoord;
            const auto deltaUV2 = v2.texCoord - v0.texCoord;

            const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            auto tangent = glm::vec3();
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            v0.tangent = tangent;
            v1.tangent = tangent;
            v2.tangent = tangent;
        }
    }

    Mesh createSphere(const U32 rings, U32 sectors){
        Mesh mesh;
        float const R = 1.0f/(float)(rings-1);
        float const S = 1.0f/(float)(sectors-1);
        U32 r, s;

        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                auto position = glm::vec3(x, y, z);
                auto normal = glm::normalize(position);
                auto tangent = glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f)));
                auto texCoord = glm::vec2(s*S, r*R);

                mesh.vertices.push_back({position, {x, y, z}, normal, tangent, texCoord});
            }

        for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back(r * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + (s+1));

            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back((r+1) * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + s);
        }
        calculateTangents(mesh);
        return mesh;
    }

    Mesh createCube(){
        Mesh cube;
        cube.vertices = {
                /// front
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// left
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// bottom
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// back
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// right
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// top
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        };
        cube.indices = {
            0, 3, 2,
            0, 2, 1,

            4, 7, 6,
            4, 6, 5,

            8, 11, 10,
            8, 10, 9,

            12, 15, 14,
            12, 14, 13,

            16, 19, 18,
            16, 18, 17,

            20, 23, 22,
            20, 22, 21
        };
        calculateTangents(cube);
        return cube;
    }



}

