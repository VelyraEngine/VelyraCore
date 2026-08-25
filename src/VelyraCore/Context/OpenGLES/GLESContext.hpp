#pragma once

#include <VelyraCore/Context/Context.hpp>

namespace Velyra::Core::GLES {

    class GLESPlatformContext;

    class GLESContext: public Context {
    public:
        GLESContext(const ContextDesc& desc, UP<GLESPlatformContext> platformContext);

        ~GLESContext() override;

        void setVerticalSynchronisation(bool enable) override;

        [[nodiscard]] bool isVerticalSynchronisationEnabled() const override;

        void swapBuffers() override;

        void makeCurrent() override;

        void createImGuiContext(const ImGuiContextDesc& desc) override;

        void DestroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

        [[nodiscard]] U32 getClientWidth() const override;

        [[nodiscard]] U32 getClientHeight() const override;

        void beginFrame() override;

        void endFrame() override;

        View<Viewport> createViewport(const ViewportDesc& desc) override;

        void destroyViewport(const View<Viewport>& viewport) override;

        View<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) override;

        View<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) override;

        void destroyShaderModule(const View<ShaderModule>& shaderModule) override;

        View<Shader> createShader(const ShaderDesc& desc) override;

        void destroyShader(const View<Shader>& shader) override;

        View<VertexLayout> createVertexLayout() override;

        void destroyVertexLayout(const View<VertexLayout>& vertexLayout) override;

        View<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) override;

        void destroyVertexBuffer(const View<VertexBuffer>& vertexBuffer) override;

        View<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) override;

        void destroyIndexBuffer(const View<IndexBuffer>& indexBuffer) override;

        View<MeshBinding> createMeshBinding(const MeshBindingDesc& desc) override;

        void destroyMeshBinding(const View<MeshBinding>& meshBinding) override;

        View<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc &desc) override;

        void destroyConstantBuffer(const View<ConstantBuffer>& constantBuffer) override;

        View<Sampler> createSampler(const SamplerDesc &desc) override;

        void destroySampler(const View<Sampler>& sampler) override;

        View<Texture2D> createTexture2D(const Texture2DDesc &desc) override;

        View<Texture2D> createTexture2D(const Texture2DImageDesc &desc) override;

        void destroyTexture2D(const View<Texture2D>& texture2D) override;

        View<FrameBufferLayout> createFrameBufferLayout() override;

        void destroyFrameBufferLayout(const View<FrameBufferLayout>& frameBufferLayout) override;

        View<FrameBuffer> createFrameBuffer(const View<FrameBufferLayout> &frameBufferLayout) override;

        void destroyFrameBuffer(const View<FrameBuffer>& frameBuffer) override;

        View<DepthStencilState> createDepthStencilState(const DepthStencilStateDesc &desc) override;

        void destroyDepthStencilState(const View<DepthStencilState>& depthStencilState) override;

        View<StructuredBuffer> createStructuredBuffer(const StructuredBufferDesc &desc) override;

        void destroyStructuredBuffer(const View<StructuredBuffer> &structuredBuffer) override;

        UP<ApiState> getState() override;

    private:

        void initGlad() const;

        void terminateGlad();

    private:
        static U64 m_ContextCount;

        UP<GLESPlatformContext> m_PlatformContext;
        Utils::LogPtr m_Logger;
    };

}