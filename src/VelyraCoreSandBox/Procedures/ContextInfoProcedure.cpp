#include "../Pch.hpp"

#include "ContextInfoProcedure.hpp"

namespace Velyra::SandBox {

    void ContextInfoProcedure::onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) {
        ImGui::Begin("Context Information");

        ImGui::Text("Graphics API: %s", Utils::toString(context->getType()).c_str());

        const Core::Device& device = context->getDevice();
        ImGui::Text("Device Name: %s", device.getDeviceName().c_str());
        ImGui::Text("Device Vendor: %s", device.getDeviceVendor().c_str());
        ImGui::Text("API Version: %s", device.getAPIVersion().c_str());
        ImGui::Text("Shading Language Version: %s", device.getShadingLanguageVersion().c_str());
        ImGui::Separator();
        ImGui::Text("Max Framebuffer Width: %u pixels", device.getMaxFramebufferWidth());
        ImGui::Text("Max Framebuffer Height: %u pixels", device.getMaxFramebufferHeight());
        ImGui::Text("Max Framebuffer Color Attachments: %u", device.getMaxFramebufferColorAttachments());
        ImGui::Text("Max Viewport Width: %u pixels", device.getMaxViewportWidth());
        ImGui::Text("Max Viewport Height: %u pixels", device.getMaxViewportHeight());
        ImGui::Text("Max Texture Slots: %u", device.getMaxTextureSlots());
        ImGui::Text("Max Texture Size: %u pixels", device.getMaxTextureSize());
        ImGui::Text("Max Constant Buffer Size: %u bytes", device.getMaxConstantBufferSize());
        ImGui::Text("Max Constant Buffer Slots: %u", device.getMaxConstantBufferSlots());
        ImGui::Text("Max Shader Storage Buffer Size: %u bytes", device.getMaxShaderStorageBufferSize());
        ImGui::Text("Max Shader Storage Buffer Slots: %u", device.getMaxShaderStorageBufferSlots());
        ImGui::Text("Max Vertex Attributes: %u", device.getMaxVertexAttributes());
        ImGui::Text("Max Vertex Count: %lu", device.getMaxVertexCount());
        ImGui::Text("Max Index Count: %lu", device.getMaxIndexCount());
        ImGui::Text("Largest index: %lu", device.getMaxIndex());
        ImGui::Text("Double Buffering Supported: %s", device.isDoubleBuffered() ? "Yes" : "No");


        ImGui::End();
    }

}
