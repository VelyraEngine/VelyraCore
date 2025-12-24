#include "../../TestPch.hpp"

#include "../../../src/VelyraCore/Window/Win32/Win32Utils.hpp"

using namespace Velyra::Core;

class TestWin32Utils : public ::testing::Test {
};



TEST_F(TestWin32Utils, ToWideString_ConvertsCorrectly) {
    const std::string input = "Hello, World!";
    const std::wstring expected = L"Hello, World!";
    const std::wstring result = toWideString(input);
    EXPECT_EQ(result, expected);
}

TEST_F(TestWin32Utils, FromWideString_ConvertsCorrectly) {
    const std::wstring input = L"Hello, World!";
    const std::string expected = "Hello, World!";
    const std::string result = fromWideString(input);
    EXPECT_EQ(result, expected);
}

