#include <gtest/gtest.h>

#include <VelyraCore/VelyraCore.hpp>

using namespace Velyra;
using namespace Velyra::Core;

class TestContext : public ::testing::Test {
};

TEST_F(TestContext, CreateOpenGLContext) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    ContextDesc contextDesc;
    contextDesc.api = VL_API_OPENGL;
    const auto& context = window->createContext(contextDesc);
    ASSERT_NE(context, nullptr);
    EXPECT_EQ(context->getType(), VL_API_OPENGL);
}

TEST_F(TestContext, OpenGLEnableDisableVSync) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    ContextDesc contextDesc;
    contextDesc.api = VL_API_OPENGL;
    const auto& context = window->createContext(contextDesc);
    ASSERT_NE(context, nullptr);

    context->setVerticalSynchronisation(true);
    EXPECT_TRUE(context->isVerticalSynchronisationEnabled());

    context->setVerticalSynchronisation(false);
    EXPECT_FALSE(context->isVerticalSynchronisationEnabled());
}
