#include "../../Pch.hpp"

#include "../../../Logging/LoggerNames.hpp"
#include "../../../Logging/GLLogging.hpp"

#include "GLContext.hpp"
#include "GLDevice.hpp"
#include "GLViewport.hpp"
#include "GLShaderModule.hpp"
#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLMeshBinding.hpp"
#include "GLConstantBuffer.hpp"
#include "GLSampler.hpp"
#include "GLTexture2D.hpp"

namespace Velyra::Core {

    template<typename T>
    void clearResources(std::vector<UP<T>>& resources) {
        resources.clear();
    }

    U64 GLContext::m_ContextCount = 0;

    GLContext::GLContext(const ContextDesc &desc, UP<GLPlatformContext> platformContext):
    Context(desc.api),
    m_PlatformContext(std::move(platformContext)),
    m_Logger(Utils::getLogger(VL_LOGGER_OGL)){
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        initGlad();
        m_Device = createUP<GLDevice>();
    }

    GLContext::~GLContext() {
        // Clear all resources before terminating glad to ensure that all OpenGL resources are properly released
        clearResources(m_Viewports);
        clearResources(m_ShaderModules);
        clearResources(m_Shaders);
        clearResources(m_VertexLayouts);
        clearResources(m_VertexBuffers);
        clearResources(m_IndexBuffers);
        clearResources(m_MeshBindings);
        clearResources(m_ConstantBuffers);
        clearResources(m_Samplers);
        clearResources(m_Texture2Ds);
        clearResources(m_RenderPassLayouts);

        terminateGlad();
    }

    void GLContext::setVerticalSynchronisation(const bool enable) {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->setVerticalSynchronisation(enable);
    }

    bool GLContext::isVerticalSynchronisationEnabled() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->isVerticalSynchronisationEnabled();
    }

    void GLContext::swapBuffers() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        VL_CORE_OPENGL_CALL(m_PlatformContext->swapBuffers())
    }

    void GLContext::makeCurrent() {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        m_PlatformContext->makeCurrent();
    }

    void GLContext::createImGuiContext(const ImGuiContextDesc &desc) {
        Context::createImGuiContext(desc);
        m_PlatformContext->initPlatformImGui(desc);
        ImGui_ImplOpenGL3_Init("#version 150");
        Context::checkImGuiFlags();
    }

    void GLContext::DestroyImGuiContext() {
        ImGui_ImplOpenGL3_Shutdown();
        m_PlatformContext->terminatePlatformImGui();
        Context::DestroyImGuiContext();
    }

    void GLContext::onImGuiBegin() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(!m_ImGuiRendering, "ImGui rendering already started!")

        ImGui_ImplOpenGL3_NewFrame();
        m_PlatformContext->onPlatformImGuiBegin();
        ImGui::NewFrame();

        m_ImGuiRendering = true;
    }

    void GLContext::onImGuiEnd() {
        VL_PRECONDITION(m_ImGuiEnabled, "There is no ImGui context created!")
        VL_PRECONDITION(m_ImGuiRendering, "ImGui rendering already started!")

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_PlatformContext->onPlatformImGuiEnd();

        m_ImGuiRendering = false;
    }

    U32 GLContext::getClientWidth() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientWidth();
    }

    U32 GLContext::getClientHeight() const {
        VL_PRECONDITION(m_PlatformContext != nullptr, "Platform context is null")

        return m_PlatformContext->getClientHeight();
    }

    void GLContext::beginFrame() {
        // Make context current if there are multiple contexts
        if (m_ContextCount > 1) {
            makeCurrent();
        }
        SPDLOG_LOGGER_TRACE(m_Logger, "===============Begin Frame===============");

        // TODO: Temporary clear, configure this with framebuffers later
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.6f, 0.8f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GLContext::endFrame() {
        SPDLOG_LOGGER_TRACE(m_Logger, "================End Frame================");
    }

    View<Viewport> GLContext::createViewport(const ViewportDesc &desc) {
        m_Viewports.emplace_back(createUP<GLViewport>(desc));
        return m_Viewports.back();
    }

    View<ShaderModule> GLContext::createShaderModule(const ShaderModuleDesc &desc) {
        m_ShaderModules.emplace_back(createUP<GLShaderModule>(desc));
        return m_ShaderModules.back();
    }

    View<ShaderModule> GLContext::createShaderModule(const ShaderModuleFileDesc &desc) {
        m_ShaderModules.emplace_back(createUP<GLShaderModule>(desc));
        return m_ShaderModules.back();
    }

    View<Shader> GLContext::createShader(const ShaderDesc &desc) {
        m_Shaders.emplace_back(createUP<GLShader>(desc));
        return m_Shaders.back();
    }

    View<VertexLayout> GLContext::createVertexLayout() {
        m_VertexLayouts.emplace_back(createUP<VertexLayout>(*m_Device));
        return m_VertexLayouts.back();
    }

    View<VertexBuffer> GLContext::createVertexBuffer(const VertexBufferDesc &desc) {
        if (desc.count > m_Device->getMaxVertexCount()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Current device supports only {} vertices, but {} requested", m_Device->getMaxVertexCount(), desc.count);
            return nullptr;
        }
        m_VertexBuffers.emplace_back(createUP<GLVertexBuffer>(desc));
        return m_VertexBuffers.back();
    }

    View<IndexBuffer> GLContext::createIndexBuffer(const IndexBufferDesc &desc) {
        if (desc.count > m_Device->getMaxIndexCount()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Current device supports only {} indices, but {} requested", m_Device->getMaxIndexCount(), desc.count);
            return nullptr;
        }
        m_IndexBuffers.emplace_back(createUP<GLIndexBuffer>(desc));
        return m_IndexBuffers.back();
    }

    View<MeshBinding> GLContext::createMeshBinding(const MeshBindingDesc &desc) {
        if (desc.vertexBuffer == nullptr) {
            SPDLOG_LOGGER_ERROR(m_Logger, "Cannot create MeshBinding: VertexBuffer is nullptr");
            return nullptr;
        }

        if (desc.indexBuffer != nullptr) {
            m_MeshBindings.emplace_back(createUP<GLIndexedMeshBinding>(desc));
        }
        else {
            m_MeshBindings.emplace_back(createUP<GLArrayMeshBinding>(desc));
        }
        return m_MeshBindings.back();
    }

    View<ConstantBuffer> GLContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        if (desc.size > m_Device->getMaxConstantBufferSize()) {
            SPDLOG_LOGGER_ERROR(m_Logger, "ConstantBuffer {}: supplied size {} exceeds the maximum constant buffer size {}",
                desc.name, desc.size, m_Device->getMaxConstantBufferSize());
            return nullptr;
        }
        m_ConstantBuffers.emplace_back(createUP<GLConstantBuffer>(desc, *m_Device));
        return m_ConstantBuffers.back();
    }

    View<Sampler> GLContext::createSampler(const SamplerDesc &desc) {
        m_Samplers.emplace_back(createUP<GLSampler>(*m_Device, desc));
        return m_Samplers.back();
    }

    View<Texture2D> GLContext::createTexture2D(const Texture2DDesc &desc) {
        m_Texture2Ds.emplace_back(createUP<GLTexture2D>(desc, *m_Device));
        return m_Texture2Ds.back();
    }

    View<Texture2D> GLContext::createTexture2D(const Texture2DImageDesc &desc) {
        m_Texture2Ds.emplace_back(createUP<GLTexture2D>(desc, *m_Device));
        return m_Texture2Ds.back();
    }

    View<RenderPassLayout> GLContext::createRenderPassLayout() {
        m_RenderPassLayouts.emplace_back(createUP<RenderPassLayout>(*m_Device));
        return m_RenderPassLayouts.back();
    }

    void GLContext::initGlad() const {
        if (m_ContextCount == 0) {
            const int version = gladLoadGL();
            if (version == 0) {
                SPDLOG_LOGGER_ERROR(m_Logger, "Failed to initialize OpenGL context");
                return;
            }
            SPDLOG_LOGGER_INFO(m_Logger, "Initialized OpenGL context with version {}.{}", GLVersion.major, GLVersion.minor);

#if defined(VL_DEBUG)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            const auto callback = static_cast<GLDEBUGPROC>(openGLMessageCallback);
            glDebugMessageCallback(callback, nullptr);
#endif
        }
        m_ContextCount++;
    }

    void GLContext::terminateGlad() {
        if (m_ContextCount == 0) {
            return;
        }
        m_ContextCount--;
        // Apparently, newer versions of glad do not have an unload function -> TODO: Investigate
    }


}