#pragma once

#include <VelyraCore/VelyraCore.hpp>

struct ShaderPackage {
    Velyra::View<Velyra::Core::ShaderModule> vertexShader;
    Velyra::View<Velyra::Core::ShaderModule> fragmentShader;
    Velyra::View<Velyra::Core::Shader> shader;
};

template<typename WRAPPER>
class Environment{
public:
    static void setup(){
        if (!m_Init) {
            using namespace Velyra;
            using namespace Velyra::Core;

            WindowDesc windowDesc;
            windowDesc.width = 800;
            windowDesc.height = 600;
            windowDesc.title = "Test Window";
            m_Window = WindowFactory::createWindow(windowDesc);

            ContextDesc contextDesc;
            contextDesc.api = m_API;
            m_Window->createContext(contextDesc);

            m_Init = true;
        }
    }

    static Velyra::Core::ShaderModuleFileDesc createShaderModuleFileDesc(const std::string& name, const VL_SHADER_TYPE type) {
        using namespace Velyra;
        using namespace Velyra::Core;

        ShaderModuleFileDesc desc;
        desc.entryPoint = "main";
        desc.shaderType = type;
        if constexpr (m_API == VL_API_OPENGL) {
            desc.filePath = fs::current_path() / "test" / "Data" / (name + "_vs.glsl");
        }
        else {
            VL_NOT_IMPLEMENTED();
        }

        return desc;
    }

    static ShaderPackage createShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
        using namespace Velyra;
        using namespace Velyra::Core;

        ShaderModuleDesc vsDesc;
        vsDesc.entryPoint = "main";
        vsDesc.shaderType = VL_SHADER_VERTEX;
        vsDesc.code = vertexShaderCode;

        ShaderModuleDesc fsDesc;
        fsDesc.entryPoint = "main";
        fsDesc.shaderType = VL_SHADER_FRAGMENT;
        fsDesc.code = fragmentShaderCode;

        ShaderPackage package;
        package.vertexShader = m_Window->getContext()->createShaderModule(vsDesc);
        package.fragmentShader = m_Window->getContext()->createShaderModule(fsDesc);

        ShaderDesc shaderDesc;
        shaderDesc.vertexShader = package.vertexShader;
        shaderDesc.fragmentShader = package.fragmentShader;
        package.shader = m_Window->getContext()->createShader(shaderDesc);

        return package;
    }

public:
    static inline Velyra::UP<Velyra::Core::Window> m_Window;
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
    static inline bool m_Init = false;

};

template<VL_GRAPHICS_API api>
struct ApiWrapper {
    static constexpr VL_GRAPHICS_API value = api;
};

using VelyraAPIS = ::testing::Types<ApiWrapper<VL_API_OPENGL>>;


