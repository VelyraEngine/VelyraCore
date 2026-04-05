#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Textures.hpp"
#include <VelyraImage/ImageFactory.hpp>

using namespace Velyra;
using namespace Velyra::Core;

template<VL_GRAPHICS_API Api, typename T>
struct Texture2DWrapper {
    static constexpr VL_GRAPHICS_API api = Api;
    using DataType = T;
};

template<typename WRAPPER>
class TestTexture2D : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<ApiWrapper<WRAPPER::api>>::setup();
    }

    template<typename T>
    UP<Image::IImage> createImage(const Size width, const Size height, Velyra::Core::Test::COLOR color) {
        if constexpr (std::is_same_v<T, U8>) {
            auto data = Velyra::Core::Test::createSquareRGBA<T>(width, height, 4, color);
            Image::ImageU8Desc desc;
            desc.data = data.data();
            desc.width = width;
            desc.height = height;
            desc.format = VL_CHANNEL_RGBA;
            return Image::ImageFactory::createImageU8(desc);
        }
        else if constexpr (std::is_same_v<T, float>) {
            auto data = Velyra::Core::Test::createSquareRGBA<T>(width, height, 4, color);
            Image::ImageF32Desc desc;
            desc.data = data.data();
            desc.width = width;
            desc.height = height;
            desc.format = VL_CHANNEL_RGBA;
            return Image::ImageFactory::createImageF32(desc);
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    void compareTextureData(const typename WRAPPER::DataType* expectedData, const View<Texture2D>& texture) {
        const auto img = texture->getData();
        auto imgData = img->getData();
        auto actualData = static_cast<const typename WRAPPER::DataType*>(imgData);
        bool equal = true;
        for (size_t i = 0; i < s_Width * s_Height * 4; ++i) {
            if (actualData[i] != expectedData[i]) {
                equal = false;
                break;
            }
        }
        EXPECT_TRUE(equal);
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::api;
    using DataType = WRAPPER::DataType;

    constexpr static Size s_Width = 50;
    constexpr static Size s_Height = 50;
};

using Texture2DTypes = ::testing::Types<
    Texture2DWrapper<VL_API_OPENGL, U8>,
    Texture2DWrapper<VL_API_OPENGL, float>
>;

TYPED_TEST_SUITE(TestTexture2D, Texture2DTypes);

TYPED_TEST(TestTexture2D, CreateTexture2DFromData) {
    auto data = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::RED);
    Texture2DDesc desc;
    desc.width = TestFixture::s_Width;
    desc.height = TestFixture::s_Height;
    desc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.data = data.data();
    auto tex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(desc);

    ASSERT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), desc.width);
    EXPECT_EQ(tex->getHeight(), desc.height);
    EXPECT_EQ(tex->getFormat(), desc.format);
    EXPECT_EQ(tex->getUsage(), desc.usage);
}

TYPED_TEST(TestTexture2D, CreateTexture2DFromImage) {
    auto image = this->template createImage<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, Test::COLOR::RED);
    Texture2DImageDesc desc;
    desc.image = image;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    auto tex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(desc);

    ASSERT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), desc.image->getWidth());
    EXPECT_EQ(tex->getHeight(), desc.image->getHeight());
    EXPECT_EQ(tex->getUsage(), desc.usage);
}

TYPED_TEST(TestTexture2D, GetData) {
    auto data = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::RED);
    Texture2DDesc desc;
    desc.width = TestFixture::s_Width;
    desc.height = TestFixture::s_Height;
    desc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.data = data.data();
    auto tex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(desc);

   this->compareTextureData(data.data(), tex);
}

TYPED_TEST(TestTexture2D, SetData) {
    auto data = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::RED);
    Texture2DDesc desc;
    desc.width = TestFixture::s_Width;
    desc.height = TestFixture::s_Height;
    desc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.data = data.data();
    auto tex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(desc);

    auto newData = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::GREEN);
    tex->setData(newData.data(), 0, 0, TestFixture::s_Width, TestFixture::s_Height);

    // Verify the data was updated correctly
    this->compareTextureData(newData.data(), tex);
}

TYPED_TEST(TestTexture2D, SetDataPartial) {
    auto data = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::RED);
    Texture2DDesc desc;
    desc.width = TestFixture::s_Width;
    desc.height = TestFixture::s_Height;
    desc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.data = data.data();
    auto tex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(desc);

    auto newData = Test::createSquareRGBA<typename TestFixture::DataType>(10, 10, 4, Test::COLOR::GREEN);
    tex->setData(newData.data(), 20, 20, 10, 10);

    // Verify the updated region
    const auto img = tex->getData();
    auto imgData = img->getData();
    auto expectedData = static_cast<const typename TestFixture::DataType*>(imgData);
    for (size_t y = 0; y < 10; ++y) {
        for (size_t x = 0; x < 10; ++x) {
            size_t index = ((20 + y) * TestFixture::s_Width + (20 + x)) * 4;
            for (size_t channel = 0; channel < 4; ++channel) {
                EXPECT_EQ(expectedData[index + channel], newData[(y * 10 + x) * 4 + channel]);
            }
        }
    }
}

TYPED_TEST(TestTexture2D, CopyFrom) {
    auto redData = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::RED);
    Texture2DDesc redDesc;
    redDesc.width = TestFixture::s_Width;
    redDesc.height = TestFixture::s_Height;
    redDesc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    redDesc.usage = VL_BUFFER_USAGE_DEFAULT;
    redDesc.data = redData.data();
    auto redTex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(redDesc);

    auto greenData = Test::createSquareRGBA<typename TestFixture::DataType>(TestFixture::s_Width, TestFixture::s_Height, 4, Test::COLOR::GREEN);
    Texture2DDesc greenDesc;
    greenDesc.width = TestFixture::s_Width;
    greenDesc.height = TestFixture::s_Height;
    greenDesc.format = Test::getFormat<typename TestFixture::DataType, 4>();
    greenDesc.usage = VL_BUFFER_USAGE_DEFAULT;
    greenDesc.data = greenData.data();
    auto greenTex = Environment<ApiWrapper<TestFixture::m_API>>::m_Window->getContext()->createTexture2D(greenDesc);

    // Now copy the green texture data into the red texture
    redTex->copyFrom(greenTex);

    // And verify that the red texture now contains the green data
    this->compareTextureData(greenData.data(), redTex);
}