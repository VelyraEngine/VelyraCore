#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include "Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

class TestContext : public ::testing::Test {
protected:
    void TearDown() override {
        // Restore the Environment's OpenGL context after creating/destroying test contexts
        if (Environment<ApiWrapper<VL_API_OPENGL>>::m_Init && 
            Environment<ApiWrapper<VL_API_OPENGL>>::m_Window) {
            Environment<ApiWrapper<VL_API_OPENGL>>::m_Window->getContext()->makeCurrent();
        }
    }
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
