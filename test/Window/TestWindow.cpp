#include "../TestPch.hpp"

#include <VelyraCore/VelyraCore.hpp>

using namespace Velyra;
using namespace Velyra::Core;

class TestWindow : public ::testing::Test {
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
    EXPECT_EQ(window->getPositionX(), desc.xPosition);
    EXPECT_EQ(window->getPositionY(), desc.yPosition);
}

TEST_F(TestWindow, ResizeWindow) {
    const auto window = WindowFactory::createWindow();
    ASSERT_NE(window, nullptr);

    window->setPosition(300, 300);
    EXPECT_EQ(window->getPositionX(), 300);
    EXPECT_EQ(window->getPositionY(), 300);
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
    EXPECT_EQ(window->getPositionX(), 400);
    EXPECT_EQ(window->getPositionY(), 400);
}
