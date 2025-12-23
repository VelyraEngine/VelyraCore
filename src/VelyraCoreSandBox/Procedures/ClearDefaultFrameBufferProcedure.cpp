#include "ClearDefaultFrameBufferProcedure.hpp"
#include "../DataContainer.hpp"

namespace Velyra::SandBox {

    void ClearDefaultFrameBufferProcedure::onUpdate(DataContainer &container, Duration deltaTime, const UP<Core::Context> &context) {
        context->beginFrame();
    }

}
