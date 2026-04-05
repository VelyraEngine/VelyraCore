#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Shaders.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestShader : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestShader, VelyraAPIS);

TYPED_TEST(TestShader, CreateShader) {
    ShaderModuleDesc vsDesc;
    vsDesc.entryPoint = "main";
    vsDesc.shaderType = VL_SHADER_VERTEX;
    vsDesc.code = Test::getSimpleVertexShader<TypeParam::value>();

    ShaderModuleDesc fsDesc;
    fsDesc.entryPoint = "main";
    fsDesc.shaderType = VL_SHADER_FRAGMENT;
    fsDesc.code = Test::getSimpleFragmentShader<TypeParam::value>();

    ShaderDesc desc;
    desc.vertexShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(vsDesc);
    desc.fragmentShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(fsDesc);

    auto shader = Environment<TypeParam>::m_Window->getContext()->createShader(desc);
    ASSERT_NE(shader, nullptr);

    shader->bind();
}

TYPED_TEST(TestShader, CreateShaderWithMissingModule) {
    ShaderModuleDesc vsDesc;
    vsDesc.entryPoint = "main";
    vsDesc.shaderType = VL_SHADER_VERTEX;
    vsDesc.code = Test::getSimpleVertexShader<TypeParam::value>();

    ShaderDesc desc;
    desc.vertexShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(vsDesc);
    desc.fragmentShader = nullptr; // Missing fragment shader

    // We still expect shader creation to return a valid object, but it should log an error internally
    auto shader = Environment<TypeParam>::m_Window->getContext()->createShader(desc);
    ASSERT_NE(shader, nullptr);
}

TYPED_TEST(TestShader, CreateShaderWithSyntaxErrorInModule) {
    ShaderModuleDesc vsDesc;
    vsDesc.entryPoint = "main";
    vsDesc.shaderType = VL_SHADER_VERTEX;
    vsDesc.code = Test::getSimpleVertexShader<TypeParam::value>();

    ShaderModuleDesc fsDesc;
    fsDesc.entryPoint = "main";
    fsDesc.shaderType = VL_SHADER_FRAGMENT;
    fsDesc.code = Test::getFragmentShaderWithError<TypeParam::value>(); // Syntax error in fragment shader

    ShaderDesc desc;
    desc.vertexShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(vsDesc);
    desc.fragmentShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(fsDesc);

    // Also here an error should be logged internally, but we expect a valid shader object
    auto shader = Environment<TypeParam>::m_Window->getContext()->createShader(desc);
    ASSERT_NE(shader, nullptr);
}

TYPED_TEST(TestShader, CreateShaderWithInterfaceMismatch) {
    ShaderModuleDesc vsDesc;
    vsDesc.entryPoint = "main";
    vsDesc.shaderType = VL_SHADER_VERTEX;
    vsDesc.code = Test::getSimpleVertexShader<TypeParam::value>();

    ShaderModuleDesc fsDesc;
    fsDesc.entryPoint = "main";
    fsDesc.shaderType = VL_SHADER_FRAGMENT;
    fsDesc.code = Test::getSimpleFragmentShaderInterfaceMisMatch<TypeParam::value>(); // Interface mismatch

    ShaderDesc desc;
    desc.vertexShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(vsDesc);
    desc.fragmentShader = Environment<TypeParam>::m_Window->getContext()->createShaderModule(fsDesc);

    // internal error logging
    auto shader = Environment<TypeParam>::m_Window->getContext()->createShader(desc);
    ASSERT_NE(shader, nullptr);
}
