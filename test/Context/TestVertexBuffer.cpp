#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"
#include "Meshes.hpp"

using namespace Velyra;
using namespace Velyra::Core;

template<typename WRAPPER>
class TestVertexBuffer : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();

        m_Layout = Environment<WRAPPER>::m_Window->getContext()->createVertexLayout();
        m_Layout->addAttribute("Position", VL_FLOAT32_3);
        m_Layout->addAttribute("Color", VL_FLOAT32_3);
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
    View<VertexLayout> m_Layout;
};

TYPED_TEST_SUITE(TestVertexBuffer, VelyraAPIS);

TYPED_TEST(TestVertexBuffer, CreateVertexBuffer) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    ASSERT_NE(vb, nullptr);
    EXPECT_EQ(vb->getCount(), 3);
    EXPECT_EQ(vb->getSize(), sizeof(float) * Test::s_TriangleVertices.size());
    EXPECT_EQ(vb->getUsage(), VL_BUFFER_USAGE_DEFAULT);
}

TYPED_TEST(TestVertexBuffer, CreateVertexBufferNoData) {
    VertexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    ASSERT_NE(vb, nullptr);
    EXPECT_EQ(vb->getCount(), 3);
    EXPECT_EQ(vb->getSize(), sizeof(float) * Test::s_TriangleVertices.size());
    EXPECT_EQ(vb->getUsage(), VL_BUFFER_USAGE_DEFAULT);
}

TYPED_TEST(TestVertexBuffer, GetData) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);
    auto data = vb->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data matches
    auto floatData = reinterpret_cast<float*>(data.data());
    for (int i = 0; i < Test::s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices[i]);
    }
}

TYPED_TEST(TestVertexBuffer, UpdateVertexBuffer) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    vb->setData(0, Test::s_TriangleVertices2.data(), 3);

    auto data = vb->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed
    auto floatData = reinterpret_cast<float*>(data.data());
    for (int i = 0; i < Test::s_TriangleVertices2.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices2[i]);
    }
}

TYPED_TEST(TestVertexBuffer, PartialUpdate) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    float newVertex[6] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // Update first vertex
    vb->setData(0, &newVertex, 1);

    auto data = vb->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed
    auto floatData = reinterpret_cast<float*>(data.data());
    EXPECT_EQ(floatData[0], -1.0f);
    EXPECT_EQ(floatData[1], -1.0f);
    EXPECT_EQ(floatData[2], 0.0f);
    EXPECT_EQ(floatData[3], 1.0f);
    EXPECT_EQ(floatData[4], 0.0f);
    EXPECT_EQ(floatData[5], 0.0f);
    for (int i = 6; i < Test::s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices[i]);
    }

    // Now update the second vertex
    vb->setData(1, &newVertex, 1);
    data = vb->getData();
    ASSERT_FALSE(data.empty());
    floatData = reinterpret_cast<float*>(data.data());
    EXPECT_EQ(floatData[0], -1.0f);
    EXPECT_EQ(floatData[1], -1.0f);
    EXPECT_EQ(floatData[2], 0.0f);
    EXPECT_EQ(floatData[3], 1.0f);
    EXPECT_EQ(floatData[4], 0.0f);
    EXPECT_EQ(floatData[5], 0.0f);
    EXPECT_EQ(floatData[6], -1.0f);
    EXPECT_EQ(floatData[7], -1.0f);
    EXPECT_EQ(floatData[8], 0.0f);
    EXPECT_EQ(floatData[9], 1.0f);
    EXPECT_EQ(floatData[10], 0.0f);
    EXPECT_EQ(floatData[11], 0.0f);
    for (int i = 12; i < Test::s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices[i]);
    }

    // Finally update the third vertex
    vb->setData(2, &newVertex, 1);
    data = vb->getData();
    ASSERT_FALSE(data.empty());
    floatData = reinterpret_cast<float*>(data.data());
    for (Size i = 0; i < 3; i++) {
        EXPECT_EQ(floatData[i * 6 + 0], -1.0f);
        EXPECT_EQ(floatData[i * 6 + 1], -1.0f);
        EXPECT_EQ(floatData[i * 6 + 2], 0.0f);
        EXPECT_EQ(floatData[i * 6 + 3], 1.0f);
        EXPECT_EQ(floatData[i * 6 + 4], 0.0f);
        EXPECT_EQ(floatData[i * 6 + 5], 0.0f);
    }
}

TYPED_TEST(TestVertexBuffer, UpdateVertexBufferStatic) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_STATIC; // should not be able to update
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    // Update should not happen
    vb->setData(0, Test::s_TriangleVertices2.data(), 3);
    auto data = vb->getData();
    ASSERT_FALSE(data.empty());
    // Check if the data has NOT changed
    auto floatData = reinterpret_cast<float*>(data.data());
    for (int i = 0; i < Test::s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices[i]);
    }
}

TYPED_TEST(TestVertexBuffer, UpdateVertexBufferLargerData) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    desc.layout = this->m_Layout;

    auto vb = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    vb->setData(0, Test::s_RectangleVertices.data(), 4);

    auto data = vb->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data has changed for the first 3 vertices
    auto floatData = reinterpret_cast<float*>(data.data());
    for (int i = 0; i < Test::s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_RectangleVertices[i]);
    }
}

TYPED_TEST(TestVertexBuffer, CopyFrom) {
    VertexBufferDesc desc;
    desc.data = Test::s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = VL_BUFFER_USAGE_DYNAMIC;
    desc.layout = this->m_Layout;

    auto vb1 = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);
    auto vb2 = Environment<TypeParam>::m_Window->getContext()->createVertexBuffer(desc);

    // Update vb2 with different data
    vb2->setData(0, Test::s_TriangleVertices2.data(), 3);

    // Copy from vb2 to vb1
    vb1->copyFrom(vb2);

    auto data = vb1->getData();
    ASSERT_FALSE(data.empty());

    // Check if the data in vb1 matches vb2
    auto floatData = reinterpret_cast<float*>(data.data());
    for (int i = 0; i < Test::s_TriangleVertices2.size(); i++){
        EXPECT_EQ(floatData[i], Test::s_TriangleVertices2[i]);
    }
}