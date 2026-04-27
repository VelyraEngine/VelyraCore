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

    EXPECT_EQ(GL_VERTEX_SHADER, getGLShaderType(VL_SHADER_TYPE_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateBufferUsage) {
    EXPECT_EQ(GL_DYNAMIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_DEFAULT));
    EXPECT_EQ(GL_DYNAMIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_DYNAMIC));
    EXPECT_EQ(GL_STATIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_STATIC));

    EXPECT_EQ(GL_STATIC_DRAW, getGLBufferUsage(VL_BUFFER_USAGE_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateDrawMode) {
    EXPECT_EQ(GL_POINTS, getGLDrawMode(VL_DRAW_POINTS));
    EXPECT_EQ(GL_LINES, getGLDrawMode(VL_DRAW_LINES));
    EXPECT_EQ(GL_LINE_STRIP, getGLDrawMode(VL_DRAW_LINES_STRIP));
    EXPECT_EQ(GL_TRIANGLES, getGLDrawMode(VL_DRAW_TRIANGLES));
    EXPECT_EQ(GL_TRIANGLE_STRIP, getGLDrawMode(VL_DRAW_TRIANGLE_STRIP));

    EXPECT_EQ(GL_TRIANGLES, getGLDrawMode(VL_DRAW_MODE_MAX_VALUE));
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

    EXPECT_EQ(GL_FLOAT, getGLDataType(VL_TYPE_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateComparisonFunc) {
    EXPECT_EQ(GL_ALWAYS, getGLComparisonFunc(VL_COMPARISON_FUNC_ALWAYS));
    EXPECT_EQ(GL_NEVER, getGLComparisonFunc(VL_COMPARISON_FUNC_NEVER));
    EXPECT_EQ(GL_LESS, getGLComparisonFunc(VL_COMPARISON_FUNC_LESS));
    EXPECT_EQ(GL_EQUAL, getGLComparisonFunc(VL_COMPARISON_FUNC_EQUAL));
    EXPECT_EQ(GL_LEQUAL, getGLComparisonFunc(VL_COMPARISON_FUNC_LESS_EQUAL));
    EXPECT_EQ(GL_GREATER, getGLComparisonFunc(VL_COMPARISON_FUNC_GREATER));
    EXPECT_EQ(GL_GEQUAL, getGLComparisonFunc(VL_COMPARISON_FUNC_GREATER_EQUAL));
    EXPECT_EQ(GL_NOTEQUAL, getGLComparisonFunc(VL_COMPARISON_FUNC_NOT_EQUAL));

    EXPECT_EQ(GL_ALWAYS, getGLComparisonFunc(VL_COMPARISON_FUNC_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateTextureFilter) {
    EXPECT_EQ(GL_LINEAR, getGLTextureFilter(VL_TEXTURE_FILTER_LINEAR));
    EXPECT_EQ(GL_NEAREST, getGLTextureFilter(VL_TEXTURE_FILTER_POINT));

    EXPECT_EQ(GL_LINEAR, getGLTextureFilter(VL_TEXTURE_FILTER_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateTextureWrap) {
    EXPECT_EQ(GL_REPEAT, getGLTextureWrap(VL_TEXTURE_WRAP_REPEAT));
    EXPECT_EQ(GL_MIRRORED_REPEAT, getGLTextureWrap(VL_TEXTURE_WRAP_MIRROR_REPEAT));
    EXPECT_EQ(GL_CLAMP_TO_EDGE, getGLTextureWrap(VL_TEXTURE_WRAP_CLAMP_EDGE));
    EXPECT_EQ(GL_CLAMP_TO_BORDER, getGLTextureWrap(VL_TEXTURE_WRAP_CLAMP_BORDER));

    EXPECT_EQ(GL_REPEAT, getGLTextureWrap(VL_TEXTURE_WRAP_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateTextureFormat) {
    EXPECT_EQ(GL_R8, getGLTextureFormat(VL_TEXTURE_R_U8));
    EXPECT_EQ(GL_R8I, getGLTextureFormat(VL_TEXTURE_R_I8));
    EXPECT_EQ(GL_R16, getGLTextureFormat(VL_TEXTURE_R_U16));
    EXPECT_EQ(GL_R16I, getGLTextureFormat(VL_TEXTURE_R_I16));
    EXPECT_EQ(GL_R32UI, getGLTextureFormat(VL_TEXTURE_R_U32));
    EXPECT_EQ(GL_R32I, getGLTextureFormat(VL_TEXTURE_R_I32));
    EXPECT_EQ(GL_R16F, getGLTextureFormat(VL_TEXTURE_R_F16));
    EXPECT_EQ(GL_R32F, getGLTextureFormat(VL_TEXTURE_R_F32));

    EXPECT_EQ(GL_RG8, getGLTextureFormat(VL_TEXTURE_RG_U8));
    EXPECT_EQ(GL_RG8I, getGLTextureFormat(VL_TEXTURE_RG_I8));
    EXPECT_EQ(GL_RG16, getGLTextureFormat(VL_TEXTURE_RG_U16));
    EXPECT_EQ(GL_RG16I, getGLTextureFormat(VL_TEXTURE_RG_I16));
    EXPECT_EQ(GL_RG32UI, getGLTextureFormat(VL_TEXTURE_RG_U32));
    EXPECT_EQ(GL_RG32I, getGLTextureFormat(VL_TEXTURE_RG_I32));
    EXPECT_EQ(GL_RG16F, getGLTextureFormat(VL_TEXTURE_RG_F16));
    EXPECT_EQ(GL_RG32F, getGLTextureFormat(VL_TEXTURE_RG_F32));

    EXPECT_EQ(GL_RGB8, getGLTextureFormat(VL_TEXTURE_RGB_U8));
    EXPECT_EQ(GL_RGB8I, getGLTextureFormat(VL_TEXTURE_RGB_I8));
    EXPECT_EQ(GL_RGB16, getGLTextureFormat(VL_TEXTURE_RGB_U16));
    EXPECT_EQ(GL_RGB16I, getGLTextureFormat(VL_TEXTURE_RGB_I16));
    EXPECT_EQ(GL_RGB32UI, getGLTextureFormat(VL_TEXTURE_RGB_U32));
    EXPECT_EQ(GL_RGB32I, getGLTextureFormat(VL_TEXTURE_RGB_I32));
    EXPECT_EQ(GL_RGB16F, getGLTextureFormat(VL_TEXTURE_RGB_F16));
    EXPECT_EQ(GL_RGB32F, getGLTextureFormat(VL_TEXTURE_RGB_F32));

    EXPECT_EQ(GL_RGBA8, getGLTextureFormat(VL_TEXTURE_RGBA_U8));
    EXPECT_EQ(GL_RGBA8I, getGLTextureFormat(VL_TEXTURE_RGBA_I8));
    EXPECT_EQ(GL_RGBA16, getGLTextureFormat(VL_TEXTURE_RGBA_U16));
    EXPECT_EQ(GL_RGBA16I, getGLTextureFormat(VL_TEXTURE_RGBA_I16));
    EXPECT_EQ(GL_RGBA32UI, getGLTextureFormat(VL_TEXTURE_RGBA_U32));
    EXPECT_EQ(GL_RGBA32I, getGLTextureFormat(VL_TEXTURE_RGBA_I32));
    EXPECT_EQ(GL_RGBA16F, getGLTextureFormat(VL_TEXTURE_RGBA_F16));
    EXPECT_EQ(GL_RGBA32F, getGLTextureFormat(VL_TEXTURE_RGBA_F32));

    EXPECT_EQ(GL_DEPTH_COMPONENT16, getGLTextureFormat(VL_TEXTURE_DEPTH_16));
    EXPECT_EQ(GL_DEPTH_COMPONENT24, getGLTextureFormat(VL_TEXTURE_DEPTH_24));
    EXPECT_EQ(GL_DEPTH_COMPONENT32, getGLTextureFormat(VL_TEXTURE_DEPTH_32));
    EXPECT_EQ(GL_DEPTH24_STENCIL8, getGLTextureFormat(VL_TEXTURE_DEPTH_24_STENCIL_8));
    EXPECT_EQ(GL_DEPTH32F_STENCIL8, getGLTextureFormat(VL_TEXTURE_DEPTH_32_STENCIL_8));

    EXPECT_EQ(GL_RGBA8, getGLTextureFormat(VL_TEXTURE_FORMAT_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateTextureChannelFormat) {
    EXPECT_EQ(GL_RED, getGLTextureChannelFormat(VL_CHANNEL_R));
    EXPECT_EQ(GL_RG, getGLTextureChannelFormat(VL_CHANNEL_RG));
    EXPECT_EQ(GL_RGB, getGLTextureChannelFormat(VL_CHANNEL_RGB));
    EXPECT_EQ(GL_RGBA, getGLTextureChannelFormat(VL_CHANNEL_RGBA));

    EXPECT_EQ(GL_RGBA, getGLTextureChannelFormat(VL_CHANNEL_FORMAT_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateStencilFunc) {
    EXPECT_EQ(GL_KEEP, getGLStencilFunc(VL_STENCIL_FUNC_KEEP));
    EXPECT_EQ(GL_ZERO, getGLStencilFunc(VL_STENCIL_FUNC_ZERO));
    EXPECT_EQ(GL_REPLACE, getGLStencilFunc(VL_STENCIL_FUNC_REPLACE));
    EXPECT_EQ(GL_INCR, getGLStencilFunc(VL_STENCIL_FUNC_INCR));
    EXPECT_EQ(GL_INCR_WRAP, getGLStencilFunc(VL_STENCIL_FUNC_INCR_WRAP));
    EXPECT_EQ(GL_DECR, getGLStencilFunc(VL_STENCIL_FUNC_DECR));
    EXPECT_EQ(GL_DECR_WRAP, getGLStencilFunc(VL_STENCIL_FUNC_DECR_WRAP));
    EXPECT_EQ(GL_INVERT, getGLStencilFunc(VL_STENCIL_FUNC_INVERT));

    EXPECT_EQ(GL_KEEP, getGLStencilFunc(VL_STENCIL_FUNC_MAX_VALUE));
}

TEST_F(TestGLTranslations, TranslateFormatDesc) {
    // Test single channel formats
    GLFormatDesc desc = getGLFormatDesc(VL_TEXTURE_R_U8);
    EXPECT_EQ(VL_UINT8, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_BYTE, desc.glDataType);
    EXPECT_EQ(GL_RED, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_R_I16);
    EXPECT_EQ(VL_INT16, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_SHORT, desc.glDataType);
    EXPECT_EQ(GL_RED, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_R_F32);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_RED, desc.glChannelFormat);

    // Test dual channel formats
    desc = getGLFormatDesc(VL_TEXTURE_RG_U8);
    EXPECT_EQ(VL_UINT8, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RG, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_BYTE, desc.glDataType);
    EXPECT_EQ(GL_RG, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_RG_F32);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RG, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_RG, desc.glChannelFormat);

    // Test triple channel formats
    desc = getGLFormatDesc(VL_TEXTURE_RGB_U8);
    EXPECT_EQ(VL_UINT8, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGB, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_BYTE, desc.glDataType);
    EXPECT_EQ(GL_RGB, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_RGB_I32);
    EXPECT_EQ(VL_INT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGB, desc.channelFormat);
    EXPECT_EQ(GL_INT, desc.glDataType);
    EXPECT_EQ(GL_RGB, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_RGB_F16);
    EXPECT_EQ(VL_FLOAT16, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGB, desc.channelFormat);
    EXPECT_EQ(GL_HALF_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_RGB, desc.glChannelFormat);

    // Test quad channel formats
    desc = getGLFormatDesc(VL_TEXTURE_RGBA_U8);
    EXPECT_EQ(VL_UINT8, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGBA, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_BYTE, desc.glDataType);
    EXPECT_EQ(GL_RGBA, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_RGBA_U32);
    EXPECT_EQ(VL_UINT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGBA, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_INT, desc.glDataType);
    EXPECT_EQ(GL_RGBA, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_RGBA_F32);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGBA, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_RGBA, desc.glChannelFormat);

    // Test depth formats
    desc = getGLFormatDesc(VL_TEXTURE_DEPTH_16);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_DEPTH_COMPONENT, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_DEPTH_24);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_DEPTH_COMPONENT, desc.glChannelFormat);

    desc = getGLFormatDesc(VL_TEXTURE_DEPTH_32);
    EXPECT_EQ(VL_FLOAT32, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_R, desc.channelFormat);
    EXPECT_EQ(GL_FLOAT, desc.glDataType);
    EXPECT_EQ(GL_DEPTH_COMPONENT, desc.glChannelFormat);

    // Test depth-stencil format
    desc = getGLFormatDesc(VL_TEXTURE_DEPTH_24_STENCIL_8);
    EXPECT_EQ(VL_UINT8, desc.dataType);
    EXPECT_EQ(VL_CHANNEL_RGBA, desc.channelFormat);
    EXPECT_EQ(GL_UNSIGNED_INT_24_8, desc.glDataType);
    EXPECT_EQ(GL_DEPTH_STENCIL, desc.glChannelFormat);

    // Test that DEPTH_32_STENCIL_8 throws an exception
    EXPECT_THROW(getGLFormatDesc(VL_TEXTURE_DEPTH_32_STENCIL_8), std::exception);
}
