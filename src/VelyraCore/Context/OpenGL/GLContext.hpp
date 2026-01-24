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

        SP<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) override;

        SP<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) override;

        SP<Shader> createShader(const ShaderDesc& desc) override;

        SP<VertexLayout> createVertexLayout() override;

        SP<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) override;

        SP<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) override;

        SP<MeshBinding> createMeshBinding(const MeshBindingDesc& desc) override;

        SP<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc &desc) override;

    private:

        void initGlad() const;

        void terminateGlad();

    private:
        static U64 m_ContextCount;

        UP<GLPlatformContext> m_PlatformContext;
        Utils::LogPtr m_Logger;
    };
}