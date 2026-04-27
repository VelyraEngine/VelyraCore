#include "FrameBufferProcedure.hpp"
#include "../ProcedureExecutor.hpp"
#include "../Meshes.hpp"

namespace Velyra::SandBox {

    void FrameBufferProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        // We use the TexturedMVP contents to render that to our framebuffer
        m_SubProcedureExecutor = createUP<ProcedureExecutor>();
        m_SubProcedureExecutor->addProcedure(TexturedMVP);
        m_SubProcedureExecutor->constructStrategy(context, window);

        createFrameBuffer(context);
        const Mesh rectangle = createRectangle(0.75f);
        m_ScreenQuadMeshBinding = loadMesh(rectangle, context);
        m_ScreenQuadShader = loadShader(context, "VertexDraw", "FrameBuffer");

        Core::DepthStencilStateDesc desc;
        desc.enableDepthTest = false;
        desc.enableStencilTest =false;
        m_DepthStencilState = context->createDepthStencilState(desc);
    }

    void FrameBufferProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_FrameBuffer->begin();
        m_Viewport->bind();
        m_FrameBuffer->clear();

        m_SubProcedureExecutor->onUpdate(deltaTime, context, window);

        m_FrameBuffer->end();

        // Now render the framebuffer to the screen
        m_DepthStencilState->bind();
        m_FrameBuffer->getColorAttachment(0)->bindShaderResource(0);
        m_ScreenQuadShader.shaderProgram->bind();
        m_ScreenQuadMeshBinding->draw();
    }

    void FrameBufferProcedure::onEvent(const Core::Event &event, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_SubProcedureExecutor->onEvent(event, context, window);
        if (event.type == VL_EVENT_WINDOW_RESIZED) {
            m_FrameBuffer->onResize(context->getClientWidth(), context->getClientHeight());
            m_Viewport->resize(context->getClientWidth(), context->getClientHeight());
        }
    }

    void FrameBufferProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_SubProcedureExecutor->onImGui(context, window);

        ImGui::Begin("FrameBuffer");

        ImGui::End();
    }

    void FrameBufferProcedure::createFrameBuffer(const UP<Core::Context> &context) {
        Core::FrameBufferColorAttachmentDesc caDesc;
        caDesc.clearColor = m_ClearColor;
        caDesc.format = VL_TEXTURE_RGBA_F32;
        caDesc.enableShaderAccess = true; // We want to sample from this texture in the shader, so we need shader access

        Core::FrameBufferDepthStencilAttachmentDesc dsDesc;
        dsDesc.format = VL_TEXTURE_DEPTH_24_STENCIL_8;
        dsDesc.enableShaderAccess = true;

        m_FrameBufferLayout = context->createFrameBufferLayout();
        m_FrameBufferLayout->setDimensions(context->getClientWidth(), context->getClientHeight());
        m_FrameBufferLayout->addColorAttachment(caDesc);
        m_FrameBufferLayout->setDepthStencilAttachment(dsDesc);

        m_FrameBuffer = context->createFrameBuffer(m_FrameBufferLayout);
        
        // Create viewport for framebuffer rendering
        Core::ViewportDesc viewportDesc;
        viewportDesc.width = context->getClientWidth();
        viewportDesc.height = context->getClientHeight();
        viewportDesc.xPosition = 0;
        viewportDesc.yPosition = 0;
        m_Viewport = context->createViewport(viewportDesc);
    }
}
