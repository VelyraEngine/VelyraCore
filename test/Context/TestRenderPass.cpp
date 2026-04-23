#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Meshes.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestRenderPass : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

    void compareColorAttachmentData(const Utils::Color& expectedColor, const View<ColorAttachment>& colorAttachment) {
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

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
    static constexpr Size m_Width = 50;
    static constexpr Size m_Height = 50;
};

TYPED_TEST_SUITE(TestRenderPass, VelyraAPIS);

TYPED_TEST(TestRenderPass, ClearFrameBuffer1CATexture) {
    auto rpLayout = Environment<TypeParam>::m_Window->getContext()->createRenderPassLayout();
    rpLayout->setDimensions(this->m_Width, this->m_Height);
    RenderPassColorAttachmentDesc caDesc;
    Utils::Color clearColor(1.0f, 0.0f, 1.0f, 1.0f);
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
    this->compareColorAttachmentData(clearColor, ca1);
}