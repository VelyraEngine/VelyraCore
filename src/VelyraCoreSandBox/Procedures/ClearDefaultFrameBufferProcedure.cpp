#include "ClearDefaultFrameBufferProcedure.hpp"

namespace Velyra::SandBox {

    void ClearDefaultFrameBufferProcedure::onUpdate(Duration deltaTime, const UP<Core::Context>& context, const UP<Core::Window>& window) {
        context->beginFrame();
    }

}
