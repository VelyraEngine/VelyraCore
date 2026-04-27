#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Meshes.hpp"
#include "Shaders.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<VL_GRAPHICS_API Api, VL_TEXTURE_FORMAT CaFormat, VL_TEXTURE_FORMAT DsFormat, bool ShaderAccess>
struct TestFrameBufferCase {
    static constexpr VL_GRAPHICS_API api = Api;
    static constexpr VL_TEXTURE_FORMAT caFormat = CaFormat;
    static constexpr VL_TEXTURE_FORMAT dsFormat = DsFormat;
    static constexpr bool shaderAccess = ShaderAccess;
};

template<typename CASE>
class TestFrameBuffer : public ::testing::Test {
protected:
    using WRAPPER = ApiWrapper<CASE::api>;

    static constexpr VL_GRAPHICS_API m_API = CASE::api;
    static constexpr VL_TEXTURE_FORMAT m_CAFormat = CASE::caFormat;
    static constexpr VL_TEXTURE_FORMAT m_DSFormat = CASE::dsFormat;
    static constexpr bool m_ShaderAccess = CASE::shaderAccess;
    static constexpr Size m_Width = 50;
    static constexpr Size m_Height = 50;

protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

    void compareColorAttachmentData(const Utils::Color& expectedColor, const View<ColorAttachment>& colorAttachment) {
        const auto img = colorAttachment->getData();
        if (img == nullptr) {
            FAIL() << "Color attachment data is null";
            return;
        }
        auto imgData = img->getData();
        if constexpr (CASE::caFormat == VL_TEXTURE_RGBA_F32) {
            auto actualData = static_cast<const float*>(imgData);
            bool equal = true;
            for (size_t i = 0; i < this->m_Width * this->m_Height * 4; ++i) {
                if (actualData[i] != expectedColor.toArray()[i % 4]) {
                    equal = false;
                    break;
                }
            }
            EXPECT_TRUE(equal);
        }
        else if constexpr (CASE::caFormat == VL_TEXTURE_RGBA_U8) {
            auto actualData = static_cast<const U8*>(imgData);
            bool equal = true;
            for (size_t i = 0; i < this->m_Width * this->m_Height * 4; ++i) {
                if (actualData[i] != static_cast<U8>(expectedColor.toArray()[i % 4] * 255)) {
                    equal = false;
                    break;
                }
            }
            EXPECT_TRUE(equal);
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    void compareDepthStencilData(const float clearDepth, const U32 clearStencil, const View<DepthStencilAttachment>& dsAttachment) {
        const auto img = dsAttachment->getData();
        if (img == nullptr) {
            FAIL() << "DepthStencilAttachment data is null";
            return;
        }
        auto imgData = img->getData();
        if constexpr (CASE::dsFormat == VL_TEXTURE_DEPTH_32) {
            auto actualData = static_cast<const float*>(imgData);
            bool equal = true;
            for (size_t i = 0; i < this->m_Width * this->m_Height; ++i) {
                if (actualData[i] != clearDepth) {
                    equal = false;
                    break;
                }
            }
            EXPECT_TRUE(equal);
        }
        else if constexpr (CASE::dsFormat == VL_TEXTURE_DEPTH_24_STENCIL_8) {
            auto actualData = static_cast<const U32*>(imgData);
            bool equal = true;
            for (size_t i = 0; i < this->m_Width * this->m_Height; ++i) {
                U32 expectedValue = (static_cast<U32>(clearDepth * ((1 << 24) - 1)) << 8) | (clearStencil & 0xFF);
                if (actualData[i] != expectedValue) {
                    equal = false;
                    break;
                }
            }
            EXPECT_TRUE(equal);
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }
};

using FrameBufferCases = ::testing::Types<
    TestFrameBufferCase<VL_API_OPENGL, VL_TEXTURE_RGBA_F32, VL_TEXTURE_DEPTH_32, true>,
    TestFrameBufferCase<VL_API_OPENGL, VL_TEXTURE_RGBA_F32, VL_TEXTURE_DEPTH_32, false>,
    TestFrameBufferCase<VL_API_OPENGL, VL_TEXTURE_RGBA_U8, VL_TEXTURE_DEPTH_24_STENCIL_8, true>,
    TestFrameBufferCase<VL_API_OPENGL, VL_TEXTURE_RGBA_U8, VL_TEXTURE_DEPTH_24_STENCIL_8, false>
>;

TYPED_TEST_SUITE(TestFrameBuffer, FrameBufferCases);

TYPED_TEST(TestFrameBuffer, ClearFrameBuffer1CA) {
    auto fbLayout = Environment<typename TestFixture::WRAPPER>::m_Window->getContext()->createFrameBufferLayout();
    fbLayout->setDimensions(this->m_Width, this->m_Height);
    FrameBufferColorAttachmentDesc caDesc;
    const Utils::Color clearColor(1.0f, 0.0f, 1.0f, 1.0f);
    caDesc.clearColor = clearColor;
    caDesc.format = this->m_CAFormat;
    caDesc.enableShaderAccess = this->m_ShaderAccess;
    caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    fbLayout->addColorAttachment(caDesc);

    // Create the Frame Buffer
    auto fb = Environment<typename TestFixture::WRAPPER>::m_Window->getContext()->createFrameBuffer(fbLayout);
    ASSERT_NE(fb, nullptr);
    const auto& ca1 = fb->getColorAttachment(0);
    ASSERT_NE(ca1, nullptr);
    EXPECT_EQ(ca1->getWidth(), this->m_Width);
    EXPECT_EQ(ca1->getHeight(), this->m_Height);
    EXPECT_EQ(ca1->getFormat(), this->m_CAFormat);
    EXPECT_EQ(ca1->getClearColor(), clearColor);

    // Now Clear it!
    fb->clear();

    // Check if the clear color is correct
    auto img = ca1->getData();
    EXPECT_EQ(img->getWidth(), this->m_Width);
    EXPECT_EQ(img->getHeight(), this->m_Height);
    this->compareColorAttachmentData(clearColor, ca1);
}

TYPED_TEST(TestFrameBuffer, ClearFrameBuffer1DS) {
    auto fbLayout = Environment<typename TestFixture::WRAPPER>::m_Window->getContext()->createFrameBufferLayout();
    fbLayout->setDimensions(this->m_Width, this->m_Height);
    FrameBufferDepthStencilAttachmentDesc dsDesc;
    dsDesc.clearDepth = 0.8f;
    dsDesc.clearStencil = 52;
    /*
     * To make life easier, we use a depth-only format so that reading the data back is much easier. We don't have to
     * do weird conversions to extract the depth and stencil values from a packed format like VL_TEXTURE_DEPTH_24_STENCIL_8.
     * The actual data does not matter anyway
     */
    dsDesc.format = this->m_DSFormat;
    dsDesc.enableShaderAccess = this->m_ShaderAccess;
    dsDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    fbLayout->setDepthStencilAttachment(dsDesc);

    // OpenGL requires also a CA to be present, so we add a dummy one that we won't use in this test
    FrameBufferColorAttachmentDesc caDesc;
    caDesc.clearColor = Utils::Color(0.0f, 0.0f, 0.0f, 1.0f);
    caDesc.format = VL_TEXTURE_RGBA_F32;
    caDesc.enableShaderAccess = false;
    caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    fbLayout->addColorAttachment(caDesc);

    // Create the Frame Buffer
    auto fb = Environment<typename TestFixture::WRAPPER>::m_Window->getContext()->createFrameBuffer(fbLayout);
    ASSERT_NE(fb, nullptr);
    const auto& dsAttachment = fb->getDepthStencilAttachment();
    ASSERT_NE(dsAttachment, nullptr);
    EXPECT_EQ(dsAttachment->getWidth(), this->m_Width);
    EXPECT_EQ(dsAttachment->getHeight(), this->m_Height);
    EXPECT_EQ(dsAttachment->getFormat(), this->m_DSFormat);
    EXPECT_EQ(dsAttachment->getClearDepth(), dsDesc.clearDepth);
    EXPECT_EQ(dsAttachment->getClearStencil(), dsDesc.clearStencil);

    // Now Clear it!
    fb->clear();

    // Check if the clear depth and stencil values are correct
    this->compareDepthStencilData(dsDesc.clearDepth, dsDesc.clearStencil, dsAttachment);
}

// TYPED_TEST(TestFrameBuffer, ClearFrameBuffer2CATexture) {
//     auto fbLayout = Environment<TypeParam>::m_Window->getContext()->createFrameBufferLayout();
//     fbLayout->setDimensions(this->m_Width, this->m_Height);
//     FrameBufferColorAttachmentDesc ca1Desc;
//     const Utils::Color clearColor1(1.0f, 0.0f, 1.0f, 1.0f);
//     ca1Desc.clearColor = clearColor1;
//     ca1Desc.format = VL_TEXTURE_RGBA_F32;
//     ca1Desc.enableShaderAccess = true;
//     ca1Desc.usage = VL_BUFFER_USAGE_DYNAMIC;
//     fbLayout->addColorAttachment(ca1Desc);
//
//     FrameBufferColorAttachmentDesc ca2Desc;
//     const Utils::Color clearColor2(0.0f, 1.0f, 1.0f, 1.0f);
//     ca2Desc.clearColor = clearColor2;
//     ca2Desc.format = VL_TEXTURE_RGBA_U8; // Intentionally use a different format to test multiple attachments with different formats
//     ca2Desc.enableShaderAccess = true;
//     ca2Desc.usage = VL_BUFFER_USAGE_DYNAMIC;
//     fbLayout->addColorAttachment(ca2Desc);
//
//     // Create the Frame Buffer
//     auto fb = Environment<TypeParam>::m_Window->getContext()->createFrameBuffer(fbLayout);
//     ASSERT_NE(fb, nullptr);
//
//     // Clear it
//     fb->clear();
//
//     // Check if the clear colors are correct
//     const auto& ca1 = fb->getColorAttachment(0);
//     ASSERT_NE(ca1, nullptr);
//     this->compareColorAttachmentDataF32(clearColor1, ca1);
//
//     const auto& ca2 = fb->getColorAttachment(1);
//     ASSERT_NE(ca2, nullptr);
//     const std::array<U8, 4> clearColor2U8 = {
//         static_cast<U8>(clearColor2.r * 255),
//         static_cast<U8>(clearColor2.g * 255),
//         static_cast<U8>(clearColor2.b * 255),
//         static_cast<U8>(clearColor2.a * 255)
//     };
//     this->compareColorAttachmentDataU8(clearColor2U8, ca2);
// }
//
// TYPED_TEST(TestFrameBuffer, DrawFrameBuffer1CATexture) {
//     // Create the screen shader
//     const ShaderPackage screenShader = Environment<TypeParam>::createShader(Test::getScreenVertexShader1CA<TypeParam::value>(), Test::getScreenFragmentShader1CA<TypeParam::value>());
//
//     // Create a simple screen rectangle
//     auto vLayout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
//     vLayout->addAttribute("Position", VL_FLOAT32_3);
//     vLayout->addAttribute("Color", VL_FLOAT32_3);
//
//     VertexBufferDesc vbDesc;
//     vbDesc.count = 4;
//     vbDesc.data = Test::s_ScreenVertices.data();
//     vbDesc.layout = vLayout;
//
//     IndexBufferDesc ibDesc;
//     ibDesc.count = 6;
//     ibDesc.data = Test::s_RectangleIndices.data();
//     ibDesc.dataType = VL_UINT32;
//
//     MeshBindingDesc mbDesc;
//     mbDesc.vertexBuffer = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(vbDesc);
//     mbDesc.indexBuffer = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(ibDesc);
//     auto screenQuad = Environment<TypeParam>::m_Window->getContext()->createMeshBinding(mbDesc);
//
//     // Now create the 1CATexture Frame Buffer
//     auto fbLayout = Environment<TypeParam>::m_Window->getContext()->createFrameBufferLayout();
//     fbLayout->setDimensions(this->m_Width, this->m_Height);
//     FrameBufferColorAttachmentDesc caDesc;
//     const Utils::Color clearColor(0.0f, 1.0f, 0.0f, 1.0f);
//     caDesc.clearColor = clearColor;
//     caDesc.format = VL_TEXTURE_RGBA_F32;
//     caDesc.enableShaderAccess = true;
//     caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
//     fbLayout->addColorAttachment(caDesc);
//
//     auto fb = Environment<TypeParam>::m_Window->getContext()->createFrameBuffer(fbLayout);
//     ASSERT_NE(fb, nullptr);
//
//     // Draw to the Frame Buffer
//     fb->begin();
//     screenShader.shader->bind();
//     screenQuad->draw();
//     fb->end();
//
//     // Check if the drawn color is correct
//     const Utils::Color expectedColor(1.0f, 1.0f, 0.0f, 1.0f); // The shader outputs yellow
//     const auto& ca1 = fb->getColorAttachment(0);
//     this->compareColorAttachmentDataF32(expectedColor, ca1);
// }
//
// TYPED_TEST(TestFrameBuffer, DrawFrameBuffer2CATexture) {
//     // Create the screen shader
//     const ShaderPackage screenShader = Environment<TypeParam>::createShader(Test::getScreenVertexShader2CA<TypeParam::value>(), Test::getScreenFragmentShader2CA<TypeParam::value>());
//
//     // Create a simple screen rectangle
//     auto vLayout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
//     vLayout->addAttribute("Position", VL_FLOAT32_3);
//     vLayout->addAttribute("Color1", VL_FLOAT32_3);
//     vLayout->addAttribute("Color2", VL_FLOAT32_3);
//
//     VertexBufferDesc vbDesc;
//     vbDesc.count = 4;
//     vbDesc.data = Test::s_ScreenVertices2.data();
//     vbDesc.layout = vLayout;
//
//     IndexBufferDesc ibDesc;
//     ibDesc.count = 6;
//     ibDesc.data = Test::s_RectangleIndices.data();
//     ibDesc.dataType = VL_UINT32;
//
//     MeshBindingDesc mbDesc;
//     mbDesc.vertexBuffer = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(vbDesc);
//     mbDesc.indexBuffer = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(ibDesc);
//     auto screenQuad = Environment<TypeParam>::m_Window->getContext()->createMeshBinding(mbDesc);
//
//     // Now create the 1CATexture Frame Buffer
//     auto fbLayout = Environment<TypeParam>::m_Window->getContext()->createFrameBufferLayout();
//     fbLayout->setDimensions(this->m_Width, this->m_Height);
//     FrameBufferColorAttachmentDesc caDesc;
//     const Utils::Color clearColor(0.0f, 1.0f, 0.0f, 1.0f);
//     caDesc.clearColor = clearColor;
//     caDesc.format = VL_TEXTURE_RGBA_F32;
//     caDesc.enableShaderAccess = true;
//     caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
//     fbLayout->addColorAttachment(caDesc);
//     FrameBufferColorAttachmentDesc caDescRb;
//     caDescRb.clearColor = clearColor;
//     caDescRb.format = VL_TEXTURE_RGBA_F32;
//     caDescRb.enableShaderAccess = false; // This will force the implementation to use a renderbuffer for this attachment
//     caDescRb.usage = VL_BUFFER_USAGE_DYNAMIC;
//     fbLayout->addColorAttachment(caDescRb); // Add a second attachment with the same properties to test multiple attachments
//
//     auto fb = Environment<TypeParam>::m_Window->getContext()->createFrameBuffer(fbLayout);
//     ASSERT_NE(fb, nullptr);
//
//     // Draw to the Frame Buffer
//     fb->begin();
//     screenShader.shader->bind();
//     screenQuad->draw();
//     fb->end();
//
//     // Check if the drawn color is correct
//     const Utils::Color expectedColor1(1.0f, 1.0f, 0.0f, 1.0f); // The shader outputs yellow
//     const auto& ca1 = fb->getColorAttachment(0);
//     this->compareColorAttachmentDataF32(expectedColor1, ca1);
//
//     const Utils::Color expectedColor2(0.0f, 0.0f, 1.0f, 1.0f); // The shader outputs blue
//     const auto& ca2 = fb->getColorAttachment(1);
//     this->compareColorAttachmentDataF32(expectedColor2, ca2);
// }
