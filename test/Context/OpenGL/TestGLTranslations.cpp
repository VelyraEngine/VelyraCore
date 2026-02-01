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

TEST_F(TestGLTranslations, TranslateBufferUsage) {
    EXPECT_EQ(GL_DYNAMIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_DEFAULT));
    EXPECT_EQ(GL_DYNAMIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_DYNAMIC));
    EXPECT_EQ(GL_STATIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_STATIC));
}

TEST_F(TestGLTranslations, TranslateDrawMode) {
    EXPECT_EQ(GL_POINTS, getGLDrawMode(VL_DRAW_POINTS));
    EXPECT_EQ(GL_LINES, getGLDrawMode(VL_DRAW_LINES));
    EXPECT_EQ(GL_LINE_STRIP, getGLDrawMode(VL_DRAW_LINES_STRIP));
    EXPECT_EQ(GL_TRIANGLES, getGLDrawMode(VL_DRAW_TRIANGLES));
    EXPECT_EQ(GL_TRIANGLE_STRIP, getGLDrawMode(VL_DRAW_TRIANGLE_STRIP));
}

TEST_F(TestGLTranslations, TranslateDataType) {
    EXPECT_EQ(GL_BYTE, getGLDataType(VL_INT8));
    EXPECT_EQ(GL_UNSIGNED_BYTE, getGLDataType(VL_UINT8));
    EXPECT_EQ(GL_SHORT, getGLDataType(VL_INT16));
    EXPECT_EQ(GL_UNSIGNED_SHORT, getGLDataType(VL_UINT16));
    EXPECT_EQ(GL_INT, getGLDataType(VL_INT32));
    EXPECT_EQ(GL_UNSIGNED_INT, getGLDataType(VL_UINT32));
    EXPECT_EQ(GL_INT64_ARB, getGLDataType(VL_INT64));
    EXPECT_EQ(GL_UNSIGNED_INT64_ARB, getGLDataType(VL_UINT64));
    EXPECT_EQ(GL_HALF_FLOAT, getGLDataType(VL_FLOAT16));
    EXPECT_EQ(GL_FLOAT, getGLDataType(VL_FLOAT32));
    EXPECT_EQ(GL_DOUBLE, getGLDataType(VL_FLOAT64));
}
