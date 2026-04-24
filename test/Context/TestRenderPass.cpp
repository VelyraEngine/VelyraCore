#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Meshes.hpp"
#include "Shaders.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestRenderPass : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

    void compareColorAttachmentDataF32(const Utils::Color& expectedColor, const View<ColorAttachment>& colorAttachment) {
        const auto img = colorAttachment->getData();
        auto imgData = img->getData();
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

    void compareColorAttachmentDataU8(const std::array<U8, 4>& expectedColor, const View<ColorAttachment>& colorAttachment) {
        const auto img = colorAttachment->getData();
        auto imgData = img->getData();
        auto actualData = static_cast<const U8*>(imgData);
        bool equal = true;
        for (size_t i = 0; i < this->m_Width * this->m_Height * 4; ++i) {
            if (actualData[i] != expectedColor[i % 4]) {
                equal = false;
                break;
            }
        }
        EXPECT_TRUE(equal);
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
    static constexpr Size m_Width = 50;
    static constexpr Size m_Height = 50;
};

TYPED_TEST_SUITE(TestRenderPass, VelyraAPIS);

TYPED_TEST(TestRenderPass, ClearRenderPass1CATexture) {
    auto rpLayout = Environment<TypeParam>::m_Window->getContext()->createRenderPassLayout();
    rpLayout->setDimensions(this->m_Width, this->m_Height);
    RenderPassColorAttachmentDesc caDesc;
    const Utils::Color clearColor(1.0f, 0.0f, 1.0f, 1.0f);
    caDesc.clearColor = clearColor;
    caDesc.format = VL_TEXTURE_RGBA_F32;
    caDesc.enableShaderAccess = true;
    caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    rpLayout->addColorAttachment(caDesc);

    // Create the Render Pass
    auto rp = Environment<TypeParam>::m_Window->getContext()->createRenderPass(rpLayout);
    ASSERT_NE(rp, nullptr);
    const auto& ca1 = rp->getColorAttachment(0);
    ASSERT_NE(ca1, nullptr);
    EXPECT_EQ(ca1->getWidth(), this->m_Width);
    EXPECT_EQ(ca1->getHeight(), this->m_Height);
    EXPECT_EQ(ca1->getFormat(), VL_TEXTURE_RGBA_F32);
    EXPECT_EQ(ca1->getClearColor(), clearColor);

    // Now Clear it!
    rp->clear();

    // Check if the clear color is correct
    auto img = ca1->getData();
    EXPECT_EQ(img->getWidth(), this->m_Width);
    EXPECT_EQ(img->getHeight(), this->m_Height);
    this->compareColorAttachmentDataF32(clearColor, ca1);
}

TYPED_TEST(TestRenderPass, ClearRenderPass2CATexture) {
    auto rpLayout = Environment<TypeParam>::m_Window->getContext()->createRenderPassLayout();
    rpLayout->setDimensions(this->m_Width, this->m_Height);
    RenderPassColorAttachmentDesc ca1Desc;
    const Utils::Color clearColor1(1.0f, 0.0f, 1.0f, 1.0f);
    ca1Desc.clearColor = clearColor1;
    ca1Desc.format = VL_TEXTURE_RGBA_F32;
    ca1Desc.enableShaderAccess = true;
    ca1Desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    rpLayout->addColorAttachment(ca1Desc);

    RenderPassColorAttachmentDesc ca2Desc;
    const Utils::Color clearColor2(0.0f, 1.0f, 1.0f, 1.0f);
    ca2Desc.clearColor = clearColor2;
    ca2Desc.format = VL_TEXTURE_RGBA_U8; // Intentionally use a different format to test multiple attachments with different formats
    ca2Desc.enableShaderAccess = true;
    ca2Desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    rpLayout->addColorAttachment(ca2Desc);

    // Create the Render Pass
    auto rp = Environment<TypeParam>::m_Window->getContext()->createRenderPass(rpLayout);
    ASSERT_NE(rp, nullptr);

    // Clear it
    rp->clear();

    // Check if the clear colors are correct
    const auto& ca1 = rp->getColorAttachment(0);
    ASSERT_NE(ca1, nullptr);
    this->compareColorAttachmentDataF32(clearColor1, ca1);

    const auto& ca2 = rp->getColorAttachment(1);
    ASSERT_NE(ca2, nullptr);
    const std::array<U8, 4> clearColor2U8 = {
        static_cast<U8>(clearColor2.r * 255),
        static_cast<U8>(clearColor2.g * 255),
        static_cast<U8>(clearColor2.b * 255),
        static_cast<U8>(clearColor2.a * 255)
    };
    this->compareColorAttachmentDataU8(clearColor2U8, ca2);
}

TYPED_TEST(TestRenderPass, DrawRenderPass1CATexture) {
    // Create the screen shader
    const ShaderPackage screenShader = Environment<TypeParam>::createShader(Test::getScreenVertexShader1CA<TypeParam::value>(), Test::getScreenFragmentShader1CA<TypeParam::value>());

    // Create a simple screen rectangle
    auto vLayout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
    vLayout->addAttribute("Position", VL_FLOAT32_3);
    vLayout->addAttribute("Color", VL_FLOAT32_3);

    VertexBufferDesc vbDesc;
    vbDesc.count = 4;
    vbDesc.data = Test::s_ScreenVertices.data();
    vbDesc.layout = vLayout;

    IndexBufferDesc ibDesc;
    ibDesc.count = 6;
    ibDesc.data = Test::s_RectangleIndices.data();
    ibDesc.dataType = VL_UINT32;

    MeshBindingDesc mbDesc;
    mbDesc.vertexBuffer = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(vbDesc);
    mbDesc.indexBuffer = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(ibDesc);
    auto screenQuad = Environment<TypeParam>::m_Window->getContext()->createMeshBinding(mbDesc);

    // Now create the 1CATexture Render Pass
    auto rpLayout = Environment<TypeParam>::m_Window->getContext()->createRenderPassLayout();
    rpLayout->setDimensions(this->m_Width, this->m_Height);
    RenderPassColorAttachmentDesc caDesc;
    const Utils::Color clearColor(0.0f, 1.0f, 0.0f, 1.0f);
    caDesc.clearColor = clearColor;
    caDesc.format = VL_TEXTURE_RGBA_F32;
    caDesc.enableShaderAccess = true;
    caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    rpLayout->addColorAttachment(caDesc);

    auto rp = Environment<TypeParam>::m_Window->getContext()->createRenderPass(rpLayout);
    ASSERT_NE(rp, nullptr);

    // Draw to the Render Pass
    rp->begin();
    screenShader.shader->bind();
    screenQuad->draw();
    rp->end();

    // Check if the drawn color is correct
    const Utils::Color expectedColor(1.0f, 1.0f, 0.0f, 1.0f); // The shader outputs yellow
    const auto& ca1 = rp->getColorAttachment(0);
    this->compareColorAttachmentDataF32(expectedColor, ca1);
}

TYPED_TEST(TestRenderPass, DrawRenderPass2CATexture) {
    // Create the screen shader
    const ShaderPackage screenShader = Environment<TypeParam>::createShader(Test::getScreenVertexShader2CA<TypeParam::value>(), Test::getScreenFragmentShader2CA<TypeParam::value>());

    // Create a simple screen rectangle
    auto vLayout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
    vLayout->addAttribute("Position", VL_FLOAT32_3);
    vLayout->addAttribute("Color1", VL_FLOAT32_3);
    vLayout->addAttribute("Color2", VL_FLOAT32_3);

    VertexBufferDesc vbDesc;
    vbDesc.count = 4;
    vbDesc.data = Test::s_ScreenVertices2.data();
    vbDesc.layout = vLayout;

    IndexBufferDesc ibDesc;
    ibDesc.count = 6;
    ibDesc.data = Test::s_RectangleIndices.data();
    ibDesc.dataType = VL_UINT32;

    MeshBindingDesc mbDesc;
    mbDesc.vertexBuffer = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(vbDesc);
    mbDesc.indexBuffer = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(ibDesc);
    auto screenQuad = Environment<TypeParam>::m_Window->getContext()->createMeshBinding(mbDesc);

    // Now create the 1CATexture Render Pass
    auto rpLayout = Environment<TypeParam>::m_Window->getContext()->createRenderPassLayout();
    rpLayout->setDimensions(this->m_Width, this->m_Height);
    RenderPassColorAttachmentDesc caDesc;
    const Utils::Color clearColor(0.0f, 1.0f, 0.0f, 1.0f);
    caDesc.clearColor = clearColor;
    caDesc.format = VL_TEXTURE_RGBA_F32;
    caDesc.enableShaderAccess = true;
    caDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
    rpLayout->addColorAttachment(caDesc);
    rpLayout->addColorAttachment(caDesc); // Add a second attachment with the same properties to test multiple attachments

    auto rp = Environment<TypeParam>::m_Window->getContext()->createRenderPass(rpLayout);
    ASSERT_NE(rp, nullptr);

    // Draw to the Render Pass
    rp->begin();
    screenShader.shader->bind();
    screenQuad->draw();
    rp->end();

    // Check if the drawn color is correct
    const Utils::Color expectedColor1(1.0f, 1.0f, 0.0f, 1.0f); // The shader outputs yellow
    const auto& ca1 = rp->getColorAttachment(0);
    this->compareColorAttachmentDataF32(expectedColor1, ca1);

    const Utils::Color expectedColor2(0.0f, 0.0f, 1.0f, 1.0f); // The shader outputs blue
    const auto& ca2 = rp->getColorAttachment(1);
    this->compareColorAttachmentDataF32(expectedColor2, ca2);
}
