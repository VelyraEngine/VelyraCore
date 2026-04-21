#pragma once

#include "Internal/GLPlatformContext.hpp"

namespace Velyra::Core {

    class GLContext: public Context {
    public:
        GLContext(const ContextDesc& desc, UP<GLPlatformContext> platformContext);

        ~GLContext() override;

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

        View<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) override;

        View<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) override;

        View<Shader> createShader(const ShaderDesc& desc) override;

        View<VertexLayout> createVertexLayout() override;

        View<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) override;

        View<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) override;

        View<MeshBinding> createMeshBinding(const MeshBindingDesc& desc) override;

        View<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc &desc) override;

        View<Sampler> createSampler(const SamplerDesc &desc) override;

        View<Texture2D> createTexture2D(const Texture2DDesc &desc) override;

        View<Texture2D> createTexture2D(const Texture2DImageDesc &desc) override;

        View<RenderPassLayout> createRenderPassLayout() override;

    private:

        void initGlad() const;

        void terminateGlad();

    private:
        static U64 m_ContextCount;

        UP<GLPlatformContext> m_PlatformContext;
        Utils::LogPtr m_Logger;
    };
}