#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestVertexLayout : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestVertexLayout, VelyraAPIS);

TYPED_TEST(TestVertexLayout, AddAttributes) {
    auto layout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
    layout->addAttribute("Position", VL_FLOAT32_3); // add 12 bytes
    layout->addAttribute("Color", VL_FLOAT32_4); // add 16 bytes
    layout->addAttribute("TexCoord", VL_FLOAT32_2); // add 8 bytes

    EXPECT_EQ(layout->getStride(), 36);
    EXPECT_EQ(layout->getAttributeCount(), 3);

    EXPECT_TRUE(layout->hasAttribute("Position"));
    EXPECT_TRUE(layout->hasAttribute("Color"));
    EXPECT_TRUE(layout->hasAttribute("TexCoord"));
}

TYPED_TEST(TestVertexLayout, AddDuplicateAttribute) {
    auto layout = Environment<TypeParam>::m_Window->getContext()->createVertexLayout();
    layout->addAttribute("Position", VL_FLOAT32_3);

    // First addition should work
    EXPECT_EQ(layout->getStride(), 12);
    EXPECT_EQ(layout->getAttributeCount(), 1);
    EXPECT_TRUE(layout->hasAttribute("Position"));

    // Second addition with the same name should log a warning and not add the attribute
    layout->addAttribute("Position", VL_FLOAT32_3);
    EXPECT_EQ(layout->getStride(), 12); // Stride should remain unchanged
    EXPECT_EQ(layout->getAttributeCount(), 1); // Attribute count should remain unchanged

}
