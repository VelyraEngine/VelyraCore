#include "../../TestPch.hpp"

#include "../../../src/VelyraCore/Context/OpenGL/Internal/GLTranslations.hpp"

using namespace Velyra;
using namespace Velyra::Core;

class TestGLTranslations : public ::testing::Test {
};

TEST_F(TestGLTranslations, TranslateShaderType) {
    EXPECT_EQ(GL_VERTEX_SHADER, getGLShaderType(VL_SHADER_VERTEX));
    EXPECT_EQ(GL_FRAGMENT_SHADER, getGLShaderType(VL_SHADER_FRAGMENT));
    EXPECT_EQ(GL_FRAGMENT_SHADER, getGLShaderType(VL_SHADER_PIXEL));
    EXPECT_EQ(GL_GEOMETRY_SHADER, getGLShaderType(VL_SHADER_GEOMETRY));
    EXPECT_EQ(GL_TESS_CONTROL_SHADER, getGLShaderType(VL_SHADER_TESSELATION_CONTROL));
    EXPECT_EQ(GL_TESS_EVALUATION_SHADER, getGLShaderType(VL_SHADER_TESSELATION_EVALUATION));
    EXPECT_EQ(GL_COMPUTE_SHADER, getGLShaderType(VL_SHADER_COMPUTE));
}
