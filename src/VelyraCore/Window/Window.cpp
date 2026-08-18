#include "../Pch.hpp"

#include <VelyraCore/Window/Window.hpp>
#include <VelyraCore/Context/Context.hpp>
#include <VelyraCore/Context/Device.hpp>
#include <VelyraCore/Context/Viewport.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>
#include <VelyraCore/Context/Shader.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>
#include <VelyraCore/Context/MeshBinding.hpp>
#include <VelyraCore/Context/ConstantBuffer.hpp>
#include <VelyraCore/Context/Sampler.hpp>
#include <VelyraCore/Context/Texture2D.hpp>
#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include <VelyraCore/Context/DepthStencilState.hpp>
#include <VelyraCore/Context/ApiState.hpp>
#include <VelyraCore/Context/ColorAttachment.hpp>
#include <VelyraCore/Context/DepthStencilAttachment.hpp>
#include <VelyraCore/Context/StructuredBuffer.hpp>

namespace Velyra::Core {

    Window::~Window() = default;

    void Window::setIcon(const fs::path &file) {
        using namespace Image;

        ImageLoadDesc desc;
        desc.fileName = file;
        desc.flipOnLoad = true;
        desc.requestedFormat = VL_CHANNEL_RGBA;
        const auto image = ImageFactory::createImage(desc);

        setIcon(*image);
    }

    const UP<Context> &Window::getContext() const {
        return m_Context;
    }

    void Window::destroyContext() {
        m_Context.reset();
    }

    Window::Window() = default;
}
