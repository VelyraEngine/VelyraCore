#include "../../Pch.hpp"

#include "GLESContext.hpp"
#include "Internal/GLESPlatformContext.hpp"

#include "../../../Logging/LoggerNames.hpp"

#include "GLESDevice.hpp"
#include "GLESFrameBuffer.hpp"
#include "GLESColorAttachment.hpp"
#include "GLESDepthStencilAttachment.hpp"
#include "GLESTexture2D.hpp"

// destroyResource erases unique_ptrs, so all resource types must be complete
#include <VelyraCore/Context/Viewport.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>
#include <VelyraCore/Context/Shader.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>
#include <VelyraCore/Context/MeshBinding.hpp>
#include <VelyraCore/Context/ConstantBuffer.hpp>
#include <VelyraCore/Context/Sampler.hpp>
#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include <VelyraCore/Context/DepthStencilState.hpp>
#include <VelyraCore/Context/StructuredBuffer.hpp>
#include <VelyraCore/Context/ApiState.hpp>

namespace Velyra::Core::GLES {

    template<typename T>
    void clearResources(std::vector<UP<T>>& resources) {
        resources.clear();
    }

    template<typename T>
    void destroyResource(std::vector<UP<T>>& resources, const View<T>& resource) {
        if (resource == nullptr) {
            return;
        }
        std::erase_if(resources, [&resource](const UP<T>& item) {
            return item.get() == resource.get();
        });
    }

    U64 GLESContext::m_ContextCount = 0;

    GLESContext::GLESContext(const ContextDesc &desc, UP<GLESPlatformContext> platformContext):
    Context(desc.api),
    m_PlatformContext(std::move(platformContext)),
    m_Logger(Utils::getLogger(VL_LOGGER_GL_ES)){
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_ContextCount++;

        initGlad();
        m_Device = createUP<GLESDevice>();
        m_DefaultFrameBuffer = createUP<GLESDefaultFrameBuffer>(desc.defaultFrameBufferDesc, *m_Device);
    }

    GLESContext::~GLESContext() {
        // Clear all resources before terminating glad to ensure that all OpenGL resources are properly released
        // clearResources(m_Viewports);
        // clearResources(m_ShaderModules);
        // clearResources(m_Shaders);
        // clearResources(m_VertexLayouts);
        // clearResources(m_VertexBuffers);
        // clearResources(m_IndexBuffers);
        // clearResources(m_MeshBindings);
        // clearResources(m_ConstantBuffers);
        // clearResources(m_Samplers);
        clearResources(m_Texture2Ds);
        // clearResources(m_FrameBufferLayouts);
        // clearResources(m_FrameBuffers);

        terminateGlad();

        m_ContextCount--;
    }

    void GLESContext::setVerticalSynchronisation(const bool enable) {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->setVerticalSynchronisation(enable);
    }

    bool GLESContext::isVerticalSynchronisationEnabled() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->isVerticalSynchronisationEnabled();
    }

    void GLESContext::swapBuffers() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

    }

    void GLESContext::makeCurrent() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->makeCurrent();
    }

    void GLESContext::createImGuiContext(const ImGuiContextDesc &desc) {
        Context::createImGuiContext(desc);
        m_PlatformContext->initPlatformImGui(desc);
        ImGui_ImplOpenGL3_Init("#version 150");
        Context::checkImGuiFlags();
    }

    void GLESContext::DestroyImGuiContext() {
        ImGui_ImplOpenGL3_Shutdown();
        m_PlatformContext->terminatePlatformImGui();
        Context::DestroyImGuiContext();
    }

    void GLESContext::onImGuiBegin() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(!m_ImGuiRendering, "ImGui rendering already started!")

        ImGui_ImplOpenGL3_NewFrame();
        m_PlatformContext->onPlatformImGuiBegin();
        ImGui::NewFrame();

        m_ImGuiRendering = true;
    }

    void GLESContext::onImGuiEnd() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(m_ImGuiRendering, "ImGui rendering already started!")

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_PlatformContext->onPlatformImGuiEnd();

        m_ImGuiRendering = false;
    }

    U32 GLESContext::getClientWidth() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientWidth();
    }

    U32 GLESContext::getClientHeight() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientHeight();
    }

    void GLESContext::beginFrame() {
        // Make context current if there are multiple contexts
        if (m_ContextCount > 1) {
            makeCurrent();
        }
        SPDLOG_LOGGER_TRACE(m_Logger, "===============Begin Frame===============");
    }

    void GLESContext::endFrame() {
        SPDLOG_LOGGER_TRACE(m_Logger, "================End Frame================");
    }

    View<Viewport> GLESContext::createViewport(const ViewportDesc &desc) {
        // m_Viewports.emplace_back(createUP<GLViewport>(desc));
        // return m_Viewports.back();
    }

    View<ShaderModule> GLESContext::createShaderModule(const ShaderModuleDesc &desc) {
        // m_ShaderModules.emplace_back(createUP<GLShaderModule>(desc));
        // return m_ShaderModules.back();
    }

    View<ShaderModule> GLESContext::createShaderModule(const ShaderModuleFileDesc &desc) {
        // m_ShaderModules.emplace_back(createUP<GLShaderModule>(desc));
        // return m_ShaderModules.back();
    }

    View<Shader> GLESContext::createShader(const ShaderDesc &desc) {
        // m_Shaders.emplace_back(createUP<GLShader>(desc));
        // return m_Shaders.back();
    }

    View<VertexLayout> GLESContext::createVertexLayout() {
        // m_VertexLayouts.emplace_back(createUP<VertexLayout>(*m_Device));
        // return m_VertexLayouts.back();
    }

    View<VertexBuffer> GLESContext::createVertexBuffer(const VertexBufferDesc &desc) {
        // m_VertexBuffers.emplace_back(createUP<GLVertexBuffer>(desc));
        // return m_VertexBuffers.back();
    }

    View<IndexBuffer> GLESContext::createIndexBuffer(const IndexBufferDesc &desc) {
        // m_IndexBuffers.emplace_back(createUP<GLIndexBuffer>(desc));
        // return m_IndexBuffers.back();
    }

    View<MeshBinding> GLESContext::createMeshBinding(const MeshBindingDesc &desc) {
        // if (desc.vertexBuffer == nullptr) {
        //     SPDLOG_LOGGER_ERROR(m_Logger, "Cannot create MeshBinding: VertexBuffer is nullptr");
        //     return nullptr;
        // }
        //
        // if (desc.indexBuffer != nullptr) {
        //     m_MeshBindings.emplace_back(createUP<GLIndexedMeshBinding>(desc));
        // }
        // else {
        //     m_MeshBindings.emplace_back(createUP<GLArrayMeshBinding>(desc));
        // }
        // return m_MeshBindings.back();
    }

    View<ConstantBuffer> GLESContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        // if (desc.size > m_Device->getMaxConstantBufferSize()) {
        //     SPDLOG_LOGGER_ERROR(m_Logger, "ConstantBuffer {}: supplied size {} exceeds the maximum constant buffer size {}",
        //         desc.name, desc.size, m_Device->getMaxConstantBufferSize());
        //     return nullptr;
        // }
        // m_ConstantBuffers.emplace_back(createUP<GLConstantBuffer>(desc, *m_Device));
        // return m_ConstantBuffers.back();
    }

    View<Sampler> GLESContext::createSampler(const SamplerDesc &desc) {
        // m_Samplers.emplace_back(createUP<GLSampler>(*m_Device, desc));
        // return m_Samplers.back();
    }

    View<Texture2D> GLESContext::createTexture2D(const Texture2DDesc &desc) {
        m_Texture2Ds.emplace_back(createUP<GLESTexture2D>(desc, *m_Device));
        return m_Texture2Ds.back();
    }

    View<Texture2D> GLESContext::createTexture2D(const Texture2DImageDesc &desc) {
        m_Texture2Ds.emplace_back(createUP<GLESTexture2D>(desc, *m_Device));
        return m_Texture2Ds.back();
    }

    View<FrameBufferLayout> GLESContext::createFrameBufferLayout() {
        // m_FrameBufferLayouts.emplace_back(createUP<FrameBufferLayout>(*m_Device));
        // return m_FrameBufferLayouts.back();
    }

    View<FrameBuffer> GLESContext::createFrameBuffer(const View<FrameBufferLayout> &frameBufferLayout) {
        // m_FrameBuffers.emplace_back(createUP<GLFrameBuffer>(frameBufferLayout, *m_Device));
        // return m_FrameBuffers.back();
    }

    View<DepthStencilState> GLESContext::createDepthStencilState(const DepthStencilStateDesc &desc) {
        // m_DepthStencilStates.emplace_back(createUP<GLDepthStencilState>(desc, *m_Device));
        // return m_DepthStencilStates.back();
    }

    View<StructuredBuffer> GLESContext::createStructuredBuffer(const StructuredBufferDesc &desc) {
        // m_StructuredBuffers.emplace_back(createUP<GLStructuredBuffer>(desc, *m_Device));
        // return m_StructuredBuffers.back();
    }

    void GLESContext::destroyStructuredBuffer(const View<StructuredBuffer> &structuredBuffer) {
        destroyResource(m_StructuredBuffers, structuredBuffer);
    }

    UP<ApiState> GLESContext::getState() {
        // return createUP<GLState>();
    }

    void GLESContext::destroyViewport(const View<Viewport>& viewport) {
        destroyResource(m_Viewports, viewport);
    }

    void GLESContext::destroyShaderModule(const View<ShaderModule>& shaderModule) {
        destroyResource(m_ShaderModules, shaderModule);
    }

    void GLESContext::destroyShader(const View<Shader>& shader) {
        destroyResource(m_Shaders, shader);
    }

    void GLESContext::destroyVertexLayout(const View<VertexLayout>& vertexLayout) {
        destroyResource(m_VertexLayouts, vertexLayout);
    }

    void GLESContext::destroyVertexBuffer(const View<VertexBuffer>& vertexBuffer) {
        destroyResource(m_VertexBuffers, vertexBuffer);
    }

    void GLESContext::destroyIndexBuffer(const View<IndexBuffer>& indexBuffer) {
        destroyResource(m_IndexBuffers, indexBuffer);
    }

    void GLESContext::destroyMeshBinding(const View<MeshBinding>& meshBinding) {
        destroyResource(m_MeshBindings, meshBinding);
    }

    void GLESContext::destroyConstantBuffer(const View<ConstantBuffer>& constantBuffer) {
        destroyResource(m_ConstantBuffers, constantBuffer);
    }

    void GLESContext::destroySampler(const View<Sampler>& sampler) {
        destroyResource(m_Samplers, sampler);
    }

    void GLESContext::destroyTexture2D(const View<Texture2D>& texture2D) {
        destroyResource(m_Texture2Ds, texture2D);
    }

    void GLESContext::destroyFrameBufferLayout(const View<FrameBufferLayout>& frameBufferLayout) {
        destroyResource(m_FrameBufferLayouts, frameBufferLayout);
    }

    void GLESContext::destroyFrameBuffer(const View<FrameBuffer>& frameBuffer) {
        destroyResource(m_FrameBuffers, frameBuffer);
    }

    void GLESContext::destroyDepthStencilState(const View<DepthStencilState>& depthStencilState) {
        destroyResource(m_DepthStencilStates, depthStencilState);
    }

    void GLESContext::initGlad() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->initGlad();
    }

    void GLESContext::terminateGlad() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->terminateGlad();
    }
    
}