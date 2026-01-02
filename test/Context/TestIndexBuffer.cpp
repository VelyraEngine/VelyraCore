#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Meshes.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestIndexBuffer : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
};

TYPED_TEST_SUITE(TestIndexBuffer, VelyraAPIS);

TYPED_TEST(TestIndexBuffer, CreateIndexBuffer) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    ASSERT_NE(ib, nullptr);
    EXPECT_EQ(ib->getCount(), 3);
    EXPECT_EQ(ib->getSize(), sizeof(U32) * Test::s_TriangleIndices.size());
    EXPECT_EQ(ib->getUsage(), VL_BUFFER_USAGE_DEFAULT);
}

TYPED_TEST(TestIndexBuffer, CreateIndexBufferNoData) {
    IndexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    ASSERT_NE(ib, nullptr);
    EXPECT_EQ(ib->getCount(), 3);
    EXPECT_EQ(ib->getSize(), sizeof(U32) * Test::s_TriangleIndices.size());
    EXPECT_EQ(ib->getUsage(), VL_BUFFER_USAGE_DEFAULT);
}

TYPED_TEST(TestIndexBuffer, GetData) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);
    auto data = ib->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data matches
    auto uintData = reinterpret_cast<U32*>(data.data());
    for (int i = 0; i < Test::s_TriangleIndices.size(); i++){
        EXPECT_EQ(uintData[i], Test::s_TriangleIndices[i]);
    }
}

TYPED_TEST(TestIndexBuffer, UpdateIndexBuffer) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    ib->setData(0, Test::s_TriangleIndices2.data(), 3);

    auto data = ib->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed
    auto uintData = reinterpret_cast<U32*>(data.data());
    for (int i = 0; i < Test::s_TriangleIndices2.size(); i++){
        EXPECT_EQ(uintData[i], Test::s_TriangleIndices2[i]);
    }
}

TYPED_TEST(TestIndexBuffer, PartialUpdate) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    // Update the first index
    U32 newIndex = 4;
    ib->setData(0, &newIndex, 1);

    auto data = ib->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed correctly
    auto uintData = reinterpret_cast<U32*>(data.data());
    EXPECT_EQ(uintData[0], newIndex);
    EXPECT_EQ(uintData[1], Test::s_TriangleIndices[1]);
    EXPECT_EQ(uintData[2], Test::s_TriangleIndices[2]);

    // Update the last index
    newIndex = 6;
    ib->setData(2, &newIndex, 1);
    data = ib->getData();
    ASSERT_FALSE(data.empty());
    uintData = reinterpret_cast<U32*>(data.data());
    EXPECT_EQ(uintData[0], 4);
    EXPECT_EQ(uintData[1], Test::s_TriangleIndices[1]);
    EXPECT_EQ(uintData[2], newIndex);

    // Update the middle index
    newIndex = 5;
    ib->setData(1, &newIndex, 1);
    data = ib->getData();
    ASSERT_FALSE(data.empty());
    uintData = reinterpret_cast<U32*>(data.data());
    EXPECT_EQ(uintData[0], 4);
    EXPECT_EQ(uintData[1], newIndex);
    EXPECT_EQ(uintData[2], 6);
}

TYPED_TEST(TestIndexBuffer, UpdateIndexBufferStatic) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_STATIC; // should not be able to update

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    // Update should not happen
    ib->setData(0, Test::s_TriangleIndices2.data(), 3);
    auto data = ib->getData();
    ASSERT_FALSE(data.empty());
    // Check if the data has NOT changed
    auto uintData = reinterpret_cast<U32*>(data.data());
    for (int i = 0; i < Test::s_TriangleIndices.size(); i++){
        EXPECT_EQ(uintData[i], Test::s_TriangleIndices[i]);
    }
}

TYPED_TEST(TestIndexBuffer, UpdateIndexBufferLargerData) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    ib->setData(0, Test::s_RectangleIndices.data(), 6);

    auto data = ib->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed for the first 3 indices
    auto uintData = reinterpret_cast<U32*>(data.data());
    for (int i = 0; i < Test::s_TriangleIndices.size(); i++){
        EXPECT_EQ(uintData[i], Test::s_RectangleIndices[i]);
    }
}

TYPED_TEST(TestIndexBuffer, CopyFrom) {
    IndexBufferDesc desc;
    desc.data = Test::s_TriangleIndices.data();
    desc.count = 3;
    desc.dataType = VL_UINT32;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;

    auto ib = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);
    auto ib2 = Environment<TypeParam>::m_Window->getContext()->createIndexBuffer(desc);

    // Update the second index buffer with different data
    ib2->setData(0, Test::s_TriangleIndices2.data(), 3);

    // Copy from ib2 to ib
    ib->copyFrom(ib2);

    auto data = ib->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data matches ib2
    auto uintData = reinterpret_cast<U32*>(data.data());
    for (int i = 0; i < Test::s_TriangleIndices2.size(); i++){
        EXPECT_EQ(uintData[i], Test::s_TriangleIndices2[i]);
    }
}
