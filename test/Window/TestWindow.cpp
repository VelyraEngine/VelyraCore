#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>
#include <VelyraCore/Context/Context.hpp>
#include <VelyraCore/Context/Device.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>
#include <VelyraCore/Context/Viewport.hpp>
#include <VelyraCore/Context/Shader.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>
#include <VelyraCore/Context/MeshBinding.hpp>
#include <VelyraCore/Context/ConstantBuffer.hpp>
#include <VelyraCore/Context/Sampler.hpp>
#include <VelyraCore/Context/Texture2D.hpp>
#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include <VelyraCore/Context/DepthStencilAttachment.hpp>
#include <VelyraCore/Context/ColorAttachment.hpp>
#include <VelyraCore/Context/DepthStencilState.hpp>
#include <VelyraCore/Context/ApiState.hpp>
#include "../Context/Environment.hpp"

using namespace Velyra;
using namespace Velyra::Core;

class TestWindow : public ::testing::Test {
protected:
    void TearDown() override {
        // Restore the Environment's OpenGL context after creating/destroying test windows
        if (Environment<ApiWrapper<VL_API_OPENGL>>::m_Init && 
            Environment<ApiWrapper<VL_API_OPENGL>>::m_Window) {
            Environment<ApiWrapper<VL_API_OPENGL>>::m_Window->getContext()->makeCurrent();
        }
    }
};

TEST_F(TestWindow, CreateWindow) {
    WindowDesc desc;
    desc.width = 1024;
    desc.height = 768;
    desc.xPosition = 100;
    desc.yPosition = 100;
    desc.title = "Test Window";
    const auto window = WindowFactory::createWindow(desc);
    ASSERT_NE(window, nullptr);

    EXPECT_EQ(window->isOpen(), true);
    EXPECT_EQ(window->isFocused(), true); // Newly created windows should be focused
    EXPECT_EQ(window->getWidth(), desc.width);
    EXPECT_EQ(window->getHeight(), desc.height);
    EXPECT_EQ(window->getTitle(), desc.title);

#if !defined(VL_PLATFORM_LINUX)
    // Wayland/X11 is flaky with window position, so we skip this test on Linux
    EXPECT_EQ(window->getPositionX(), desc.xPosition);
    EXPECT_EQ(window->getPositionY(), desc.yPosition);
#endif
}

TEST_F(TestWindow, ResizeWindow) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    window->setPosition(300, 300);
#if !defined(VL_PLATFORM_LINUX)
    EXPECT_EQ(window->getPositionX(), 300);
    EXPECT_EQ(window->getPositionY(), 300);
#endif
}

TEST_F(TestWindow, SetTitle) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    const std::string newTitle = "New Window Title";
    window->setTitle(newTitle);
    EXPECT_EQ(window->getTitle(), newTitle);
}

TEST_F(TestWindow, UpdatePosition) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    window->setPosition(400, 400);
#if !defined(VL_PLATFORM_LINUX)
    EXPECT_EQ(window->getPositionX(), 400);
    EXPECT_EQ(window->getPositionY(), 400);
#endif
}
