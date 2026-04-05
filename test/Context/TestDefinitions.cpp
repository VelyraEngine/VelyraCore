#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include <VelyraImage/ImageFactory.hpp>

using namespace Velyra;
using namespace Velyra::Core;

class TestDefinitions : public ::testing::Test {
};

TEST_F(TestDefinitions, GetTextureFormat) {
    // Test single channel formats
    {
        Image::ImageU8Desc desc;
        desc.width = 1;
        desc.height = 1;
        desc.format = VL_CHANNEL_R;
        auto image = Image::ImageFactory::createImageU8(desc);
        EXPECT_EQ(getTextureFormat(image), VL_TEXTURE_R_U8);
    }
    
    {
        Image::ImageU8Desc desc;
        desc.width = 1;
        desc.height = 1;
        desc.format = VL_CHANNEL_RGB;
        auto image = Image::ImageFactory::createImageU8(desc);
        EXPECT_EQ(getTextureFormat(image), VL_TEXTURE_RGB_U8);
    }
    
    {
        Image::ImageF32Desc desc;
        desc.width = 1;
        desc.height = 1;
        desc.format = VL_CHANNEL_RGBA;
        auto image = Image::ImageFactory::createImageF32(desc);
        EXPECT_EQ(getTextureFormat(image), VL_TEXTURE_RGBA_F32);
    }
    {
        Image::ImageF32Desc desc;
        desc.width = 1;
        desc.height = 1;
        desc.format = VL_CHANNEL_R;
        auto image = Image::ImageFactory::createImageF32(desc);
        EXPECT_EQ(getTextureFormat(image), VL_TEXTURE_R_F32);
    }
}

TEST_F(TestDefinitions, GetTextureDataType) {
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_U8), VL_UINT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_I8), VL_INT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_U16), VL_UINT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_I16), VL_INT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_U32), VL_UINT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_I32), VL_INT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_F16), VL_FLOAT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_R_F32), VL_FLOAT32);

    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_U8), VL_UINT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_I8), VL_INT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_U16), VL_UINT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_I16), VL_INT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_U32), VL_UINT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_I32), VL_INT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_F16), VL_FLOAT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RG_F32), VL_FLOAT32);

    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_U8), VL_UINT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_I8), VL_INT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_U16), VL_UINT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_I16), VL_INT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_U32), VL_UINT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_I32), VL_INT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_F16), VL_FLOAT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGB_F32), VL_FLOAT32);

    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_U8), VL_UINT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_I8), VL_INT8);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_U16), VL_UINT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_I16), VL_INT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_U32), VL_UINT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_I32), VL_INT32);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_F16), VL_FLOAT16);
    EXPECT_EQ(getTextureDataType(VL_TEXTURE_RGBA_F32), VL_FLOAT32);
}

TEST_F(TestDefinitions, GetTextureChannelFormat) {
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_U8), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_I8), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_U16), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_I16), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_U32), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_I32), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_F16), VL_CHANNEL_R);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_R_F32), VL_CHANNEL_R);

    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_U8), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_I8), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_U16), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_I16), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_U32), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_I32), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_F16), VL_CHANNEL_RG);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RG_F32), VL_CHANNEL_RG);

    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_U8), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_I8), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_U16), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_I16), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_U32), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_I32), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_F16), VL_CHANNEL_RGB);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGB_F32), VL_CHANNEL_RGB);

    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_U8), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_I8), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_U16), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_I16), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_U32), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_I32), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_F16), VL_CHANNEL_RGBA);
    EXPECT_EQ(getTextureChannelFormat(VL_TEXTURE_RGBA_F32), VL_CHANNEL_RGBA);
}

