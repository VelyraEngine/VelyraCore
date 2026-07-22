#include "../TestPch.hpp"

#include <VelyraCore/Context/StructuredBuffer.hpp>
#include "Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestStructuredBuffer : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestStructuredBuffer, VelyraAPIS);

TYPED_TEST(TestStructuredBuffer, CreateStructuredBufferVertexShader) {
    constexpr Vec4 data;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_VERTEX;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestStructuredBuffer, CreateStructuredBufferFragmentShader) {
    constexpr Vec4 data;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestStructuredBuffer, CreateStructuredBufferNoData) {
    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = nullptr;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_VERTEX;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestStructuredBuffer, GetData) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);

    auto retrievedData = cb->getData();
    auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());

    EXPECT_EQ(data, *cbData);
}

TYPED_TEST(TestStructuredBuffer, UpdateStructuredBuffer) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);

    Vec4 newData;
    data.x = 10.0f;
    data.y = 20.0f;
    data.z = 30.0f;
    data.w = 40.0f;

    cb->setData(0, &newData, sizeof(Vec4));

    auto retrievedData = cb->getData();
    ASSERT_FALSE(retrievedData.empty());
    const auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());
    EXPECT_EQ(newData, *cbData);
}

TYPED_TEST(TestStructuredBuffer, PartialUpdate) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);

    float newData = 1000.0f;

    // Now only update the position
    cb->setData(sizeof(float) * 2, &newData, sizeof(float));

    auto retrievedData = cb->getData();
    ASSERT_FALSE(retrievedData.empty());
    Vec4 expectedData = data;
    expectedData.z = newData;
    const auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());
    EXPECT_EQ(expectedData, *cbData);
}

TYPED_TEST(TestStructuredBuffer, UpdateStructuredBufferStatic) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_STATIC;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);

    Vec4 newData;
    newData.x = 10.0f;
    newData.y = 20.0f;
    newData.z = 30.0f;
    newData.w = 40.0f;

    // Update should not happen
    cb->setData(0, &newData, sizeof(Vec4));

    auto retrievedData = cb->getData();
    ASSERT_FALSE(retrievedData.empty());
    const auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());
    EXPECT_EQ(data, *cbData);
}

TYPED_TEST(TestStructuredBuffer, UpdateStructuredBufferLargerData) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb, nullptr);

    Vec4 newData;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;
    Vec4 newDataArray[2] = {newData, data};
    // Update should partially happen, only the first CBData should be copied
    cb->setData(0, newDataArray, sizeof(newDataArray));
    auto retrievedData = cb->getData();
    ASSERT_FALSE(retrievedData.empty());
    const auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());
    EXPECT_EQ(newData, *cbData);
}

TYPED_TEST(TestStructuredBuffer, CopyFrom) {
    Vec4 data;
    data.x = 1.0f;
    data.y = 2.0f;
    data.z = 3.0f;
    data.w = 4.0f;

    StructuredBufferDesc desc;
    desc.size = sizeof(Vec4);
    desc.data = &data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb1 = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);
    desc.data = nullptr;
    auto cb2 = Environment<TypeParam>::m_Window->getContext()->createStructuredBuffer(desc);

    ASSERT_NE(cb1, nullptr);
    ASSERT_NE(cb2, nullptr);

    // Copy data from cb1 to cb2
    cb2->copyFrom(cb1);
    auto retrievedData = cb2->getData();
    ASSERT_FALSE(retrievedData.empty());
    const auto cbData = reinterpret_cast<Vec4*>(retrievedData.data());
    EXPECT_EQ(data, *cbData);
}