#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

struct Vec4{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    bool operator==(const Vec4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
};

struct Matrix4x4{
    Vec4 rows[4] = { 0 };

    bool operator==(const Matrix4x4& other) const {
        for (int i = 0; i < 4; i++) {
            if (rows[i] != other.rows[i]) {
                return false;
            }
        }
        return true;
    }
};

struct CBData{
    Matrix4x4 view = { 0 };
    Vec4 position = { 0 };

    bool operator==(const CBData& other) const {
        return view == other.view && position == other.position;
    }
};

template<typename WRAPPER>
class TestConstantBuffer : public ::testing::Test {
protected:
    void SetUp() override {
        Environment<WRAPPER>::setup();

        m_Data.position.x = 1.0f;
        m_Data.position.y = 2.0f;
        m_Data.position.z = 3.0f;
        m_Data.position.w = 4.0f;

        for (int i = 0; i < 4; i++) {
            m_Data.view.rows[i].x = i;
            m_Data.view.rows[i].y = i + 1;
            m_Data.view.rows[i].z = i + 2;
            m_Data.view.rows[i].w = i + 3;
        }
    }

protected:
    static constexpr VL_GRAPHICS_API m_API = WRAPPER::value;
    CBData m_Data;
};

TYPED_TEST_SUITE(TestConstantBuffer, VelyraAPIS);

TYPED_TEST(TestConstantBuffer, CreateConstantBufferVertexShader) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_VERTEX;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getName(), desc.name);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestConstantBuffer, CreateConstantBufferFragmentShader) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getName(), desc.name);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestConstantBuffer, CreateConstantBufferNoData) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = nullptr;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_VERTEX;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);
    EXPECT_EQ(cb->getName(), desc.name);
    EXPECT_EQ(cb->getSize(), desc.size);
    EXPECT_EQ(cb->getUsage(), desc.usage);
    EXPECT_EQ(cb->getShaderStage(), desc.shaderStage);
}

TYPED_TEST(TestConstantBuffer, GetData) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);

    auto data = cb->getData();
    auto cbData = reinterpret_cast<CBData*>(data.data());

    EXPECT_EQ(this->m_Data, *cbData);
}

TYPED_TEST(TestConstantBuffer, UpdateConstantBuffer) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);

    CBData newData;
    newData.position.x = 10.0f;
    newData.position.y = 20.0f;
    newData.position.z = 30.0f;
    newData.position.w = 40.0f;
    for (int i = 0; i < 4; i++) {
        newData.view.rows[i].x = i + 10;
        newData.view.rows[i].y = i + 11;
        newData.view.rows[i].z = i + 12;
        newData.view.rows[i].w = i + 13;
    }

    cb->setData(0, &newData, sizeof(CBData));

    auto data = cb->getData();
    ASSERT_FALSE(data.empty());
    const auto cbData = reinterpret_cast<CBData*>(data.data());
    EXPECT_EQ(newData, *cbData);
}

TYPED_TEST(TestConstantBuffer, PartialUpdate) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);

    CBData newData = this->m_Data;
    newData.position = { 10.0f, 20.0f, 30.0f, 40.0f };

    // Now only update the position
    cb->setData(sizeof(Matrix4x4), &newData.position, sizeof(Vec4));

    auto data = cb->getData();
    ASSERT_FALSE(data.empty());
    const auto cbData = reinterpret_cast<CBData*>(data.data());
    EXPECT_EQ(newData, *cbData);
}

TYPED_TEST(TestConstantBuffer, UpdateConstantBufferStatic) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_STATIC;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);

    CBData newData;
    newData.position.x = 10.0f;
    newData.position.y = 20.0f;
    newData.position.z = 30.0f;
    newData.position.w = 40.0f;
    for (int i = 0; i < 4; i++) {
        newData.view.rows[i].x = i + 10;
        newData.view.rows[i].y = i + 11;
        newData.view.rows[i].z = i + 12;
        newData.view.rows[i].w = i + 13;
    }

    // Update should not happen
    cb->setData(0, &newData, sizeof(CBData));

    auto data = cb->getData();
    ASSERT_FALSE(data.empty());
    const auto cbData = reinterpret_cast<CBData*>(data.data());
    EXPECT_EQ(this->m_Data, *cbData);
}

TYPED_TEST(TestConstantBuffer, UpdateConstantBufferLargerData) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb, nullptr);

    CBData newData;
    newData.position.x = 10.0f;
    newData.position.y = 20.0f;
    newData.position.z = 30.0f;
    newData.position.w = 40.0f;
    for (int i = 0; i < 4; i++) {
        newData.view.rows[i].x = i + 10;
        newData.view.rows[i].y = i + 11;
        newData.view.rows[i].z = i + 12;
        newData.view.rows[i].w = i + 13;
    }

    CBData newDataArray[] = { newData, newData };
    // Update should partially happen, only the first CBData should be copied
    cb->setData(0, newDataArray, sizeof(newDataArray));
    auto data = cb->getData();
    ASSERT_FALSE(data.empty());
    const auto cbData = reinterpret_cast<CBData*>(data.data());
    EXPECT_EQ(newData, *cbData);
}

TYPED_TEST(TestConstantBuffer, CopyFrom) {
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &this->m_Data;
    desc.usage = VL_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = VL_SHADER_FRAGMENT;
    auto cb1 = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);
    desc.data = nullptr;
    auto cb2 = Environment<TypeParam>::m_Window->getContext()->createConstantBuffer(desc);

    ASSERT_NE(cb1, nullptr);
    ASSERT_NE(cb2, nullptr);

    // Copy data from cb1 to cb2
    cb2->copyFrom(cb1);
    auto data = cb2->getData();
    ASSERT_FALSE(data.empty());
    const auto cbData = reinterpret_cast<CBData*>(data.data());
    EXPECT_EQ(this->m_Data, *cbData);
}
