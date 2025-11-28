#include <iostream>
#include "src/VelyraCoreSandBox/SandBox.hpp"

using namespace Velyra;
using namespace Velyra::Utils;
using namespace Velyra::Core;

int main(const int argc, const char* argv[]) {
    const std::vector<std::string_view> args(argv, argv + argc);
    try {
        SandBox::SandBox sandbox(args);
        sandbox.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
