#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestSampler : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestSampler, VelyraAPIS);

TYPED_TEST(TestSampler, CreateSampler) {
    SamplerDesc desc;
    desc.minFilter = VL_TEXTURE_FILTER_LINEAR;
    desc.magFilter = VL_TEXTURE_FILTER_LINEAR;
    desc.wrapU = VL_TEXTURE_WRAP_REPEAT;
    desc.wrapV = VL_TEXTURE_WRAP_REPEAT;
    desc.wrapW = VL_TEXTURE_WRAP_REPEAT;
    desc.comparisonFunc = VL_COMPARISON_FUNC_LESS_EQUAL;
    desc.borderColorR = 1.0f;
    desc.borderColorG = 0.0f;
    desc.borderColorB = 0.0f;
    desc.borderColorA = 1.0f;
    desc.minLOD = 0.0f;
    desc.maxLOD = 100.0f;
    desc.LODBias = 0.5f;

    auto sampler = Environment<TypeParam>::m_Window->getContext()->createSampler(desc);
    ASSERT_NE(sampler, nullptr);

    // Verify that the sampler properties are correctly set
    EXPECT_EQ(sampler->getMinFilter(), VL_TEXTURE_FILTER_LINEAR);
    EXPECT_EQ(sampler->getMagFilter(), VL_TEXTURE_FILTER_LINEAR);
    EXPECT_EQ(sampler->getWrapU(), VL_TEXTURE_WRAP_REPEAT);
    EXPECT_EQ(sampler->getWrapV(), VL_TEXTURE_WRAP_REPEAT);
    EXPECT_EQ(sampler->getWrapW(), VL_TEXTURE_WRAP_REPEAT);
    EXPECT_EQ(sampler->getComparisonFunc(), VL_COMPARISON_FUNC_LESS_EQUAL);
    const float* borderColor = sampler->getBorderColor();
    EXPECT_FLOAT_EQ(borderColor[0], 1.0f);
    EXPECT_FLOAT_EQ(borderColor[1], 0.0f);
    EXPECT_FLOAT_EQ(borderColor[2], 0.0f);
    EXPECT_FLOAT_EQ(borderColor[3], 1.0f);
    EXPECT_FLOAT_EQ(sampler->getMinLOD(), 0.0f);
    EXPECT_FLOAT_EQ(sampler->getMaxLOD(), 100.0f);
    EXPECT_FLOAT_EQ(sampler->getLODBias(), 0.5f);
}