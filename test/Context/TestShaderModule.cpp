#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"

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

    if constexpr (this->m_API == VL_API_OPENGL) {
        desc.code =
                "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main() {\n"
                "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n";
    }
    else {
        VL_NOT_IMPLEMENTED();
    }
    auto shaderModule = Environment<TypeParam>::m_Window->getContext()->createShaderModule(desc);
    ASSERT_NE(shaderModule, nullptr);
    EXPECT_EQ(shaderModule->getShaderModuleType(), VL_SHADER_FRAGMENT);
}

TYPED_TEST(TestShaderModule, SyntaxError) {
    ShaderModuleDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = VL_SHADER_FRAGMENT;

    if constexpr (this->m_API == VL_API_OPENGL) {
        desc.code =
                "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main() \n" // Missing opening brace
                "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n";
    }
    else {
        VL_NOT_IMPLEMENTED();
    }
    auto shaderModule = Environment<TypeParam>::m_Window->getContext()->createShaderModule(desc);
    ASSERT_NE(shaderModule, nullptr);
}