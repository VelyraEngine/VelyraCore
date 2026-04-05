#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Shaders.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestShaderModule : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestShaderModule, VelyraAPIS);

TYPED_TEST(TestShaderModule, CreateShaderModuleFromFile) {
    auto desc = Environment<TypeParam>::createShaderModuleFileDesc("VertexBuffer", VL_SHADER_VERTEX);

    auto shaderModule = Environment<TypeParam>::m_Window->getContext()->createShaderModule(desc);
    ASSERT_NE(shaderModule, nullptr);
    EXPECT_EQ(shaderModule->getShaderModuleType(), VL_SHADER_VERTEX);
    EXPECT_EQ(shaderModule->getEntryPoint(), "main");
}

TYPED_TEST(TestShaderModule, CreateShaderModuleFromString) {
    ShaderModuleDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = VL_SHADER_FRAGMENT;
    desc.code = Test::getSimpleFragmentShader<TypeParam::value>();

    auto shaderModule = Environment<TypeParam>::m_Window->getContext()->createShaderModule(desc);
    ASSERT_NE(shaderModule, nullptr);
    EXPECT_EQ(shaderModule->getShaderModuleType(), VL_SHADER_FRAGMENT);
}

TYPED_TEST(TestShaderModule, SyntaxError) {
    ShaderModuleDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = VL_SHADER_FRAGMENT;
    desc.code = Test::getFragmentShaderWithError<TypeParam::value>();

    auto shaderModule = Environment<TypeParam>::m_Window->getContext()->createShaderModule(desc);
    ASSERT_NE(shaderModule, nullptr);
}