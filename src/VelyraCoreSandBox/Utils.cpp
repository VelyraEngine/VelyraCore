#include "Utils.hpp"

namespace Velyra::SandBox {

    SP<Core::VertexLayout> Vertex::getVertexLayout(const UP<Core::Context> &context) {
        SP<Core::VertexLayout> layout = context->createVertexLayout();
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


}

