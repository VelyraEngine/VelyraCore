#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraCore/Context/FrameBuffer.hpp>

namespace Velyra::Core {

    class Viewport;
    class ViewportDesc;
    class ShaderModule;
    struct ShaderModuleDesc;
    struct ShaderModuleFileDesc;
    class Shader;
    struct ShaderDesc;
    class VertexLayout;
    class VertexBuffer;
    struct VertexBufferDesc;
    class IndexBuffer;
    struct IndexBufferDesc;
    class MeshBinding;
    struct MeshBindingDesc;
    class ConstantBuffer;
    struct ConstantBufferDesc;
    class Sampler;
    struct SamplerDesc;
    class Texture2D;
    struct Texture2DDesc;
    struct Texture2DImageDesc;
    class FrameBufferLayout;
    class DepthStencilState;
    struct DepthStencilStateDesc;
    class Device;
    class ApiState;

    struct VL_API ContextDesc {
        VL_GRAPHICS_API api = VL_API_BEST;
        U8 depthBits        = 24;
        U8 stencilBits      = 8;
        U8 redBits          = 8;
        U8 blueBits         = 8;
        U8 greenBits        = 8;
        U8 alphaBits        = 8;
        DefaultFrameBufferDesc defaultFrameBufferDesc;
    };

    struct VL_API ImGuiContextDesc {
        VL_IMGUI_STYLE style = VL_IMGUI_STYLE_DEFAULT;
        bool useImPlot = true;
        bool useDocking = false;
        bool useViewports = false;
    };

    class VL_API Context {
    public:
        virtual ~Context() = default;

        [[nodiscard]] VL_GRAPHICS_API getType() const {
            return m_Type;
        }

        [[nodiscard]] bool isImGuiEnabled() const {
            return m_ImGuiEnabled;
        }

        [[nodiscard]] bool isImPlotEnabled() const {
            return m_ImGuiDesc.useImPlot;
        }

        [[nodiscard]] const Device& getDevice() const {
            VL_PRECONDITION(m_Device != nullptr, "Device is null")

            return *m_Device;
        }

        /**
         * @brief Enables or disables vertical synchronization (V-Sync) for the context. When enabled, the context
         *        synchronizes its frame rate with the display's refresh rate to prevent screen tearing.
         * @param enable - true to enable V-Sync, false to disable it.
         */
        virtual void setVerticalSynchronisation(bool enable) = 0;

        /**
         * @brief Checks if vertical synchronization (V-Sync) is currently enabled for the context.
         * @return
         */
        [[nodiscard]] virtual bool isVerticalSynchronisationEnabled() const = 0;

        /**
         * @brief Swaps the front and back buffers of the context, presenting the rendered image to the display.
         */
        virtual void swapBuffers() = 0;

        /**
         * @brief Makes the context the current context for rendering operations. Normally this functionality is
         *        handled automatically, but in some cases, for example when using multiple OpenGL contexts, it might
         *        be necessary to manually set the current context.
         */
        virtual void makeCurrent() = 0;

        /**
         * @brief Creates an ImGui context for the current rendering context with the specified configuration.
         * @param desc
         */
        virtual void createImGuiContext(const ImGuiContextDesc& desc);

        void checkImGuiFlags() const;

        virtual void DestroyImGuiContext();

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

        /**
         * @brief Returns the width of the client area in pixels. The client area is the drawable part of the window
         *        (without borders, title bar, etc.).
         * @return Width of the client area in pixels.
         */
        [[nodiscard]] virtual U32 getClientWidth() const = 0;

        /**
         * @brief Returns the width of the client area in pixels. The client area is the drawable part of the window
         *        (without borders, title bar, etc.).
         * @return Width of the client area in pixels.
         */
        [[nodiscard]] virtual U32 getClientHeight() const = 0;

        virtual void beginFrame() = 0;

        virtual void endFrame() = 0;

        /**
         * @brief Creates a viewport
         * @param desc
         * @return
         */
        virtual View<Viewport> createViewport(const ViewportDesc& desc) = 0;

        /**
         * @brief Destroys a viewport
         * @param viewport The viewport to destroy
         */
        virtual void destroyViewport(const View<Viewport>& viewport) = 0;

        /**
         * @brief Creates a shader module and compiles the code from a std::string.
         * @param desc
         * @return
         */
        virtual View<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) = 0;

        /**
         * @brief Creates a shader module and compiles the code from a file.
         * @param desc
         * @return
         */
        virtual View<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) = 0;

        /**
         * @brief Destroys a shader module
         * @param shaderModule The shader module to destroy
         */
        virtual void destroyShaderModule(const View<ShaderModule>& shaderModule) = 0;

        /**
         * @brief Creates a shader given a valid vertex and fragment shader module.
         *        (with extra optional shaders)
         * @param desc
         * @return
         */
        virtual View<Shader> createShader(const ShaderDesc& desc) = 0;

        /**
         * @brief Destroys a shader
         * @param shader The shader to destroy
         */
        virtual void destroyShader(const View<Shader>& shader) = 0;

        /**
         * @brief Creates a vertex layout object, used to configure vertex buffers.
         * @return
         */
        virtual View<VertexLayout> createVertexLayout() = 0;

        /**
         * @brief Destroys a vertex layout
         * @param vertexLayout The vertex layout to destroy
         */
        virtual void destroyVertexLayout(const View<VertexLayout>& vertexLayout) = 0;

        /**
         * @brief Creates a vertex buffer given a valid description.
         * @param desc Description of the vertex buffer
         * @return
         */
        virtual View<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) = 0;

        /**
         * @brief Destroys a vertex buffer
         * @param vertexBuffer The vertex buffer to destroy
         */
        virtual void destroyVertexBuffer(const View<VertexBuffer>& vertexBuffer) = 0;

        /**
         * @brief Creates an index buffer given a valid description.
         * @param desc Description of the index buffer
         * @return
         */
        virtual View<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) = 0;

        /**
         * @brief Destroys an index buffer
         * @param indexBuffer The index buffer to destroy
         */
        virtual void destroyIndexBuffer(const View<IndexBuffer>& indexBuffer) = 0;

        /**
         * @brief Creates a mesh binding given a valid description.
         * @param desc
         * @return
         */
        virtual View<MeshBinding> createMeshBinding(const MeshBindingDesc& desc) = 0;

        /**
         * @brief Destroys a mesh binding
         * @param meshBinding The mesh binding to destroy
         */
        virtual void destroyMeshBinding(const View<MeshBinding>& meshBinding) = 0;

        /**
         * @brief Creates a constant buffer given a valid description.
         * @param desc Description of the constant buffer
         * @return
         */
        virtual View<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        /**
         * @brief Destroys a constant buffer
         * @param constantBuffer The constant buffer to destroy
         */
        virtual void destroyConstantBuffer(const View<ConstantBuffer>& constantBuffer) = 0;

        /**
         * @brief Creates a sampler given a valid description.
         * @param desc Description of the sampler
         * @return
         */
        virtual View<Sampler> createSampler(const SamplerDesc& desc) = 0;

        /**
         * @brief Destroys a sampler
         * @param sampler The sampler to destroy
         */
        virtual void destroySampler(const View<Sampler>& sampler) = 0;

        /**
         * @brief Creates a 2D texture from raw data
         * @param desc
         * @return
         */
        virtual View<Texture2D> createTexture2D(const Texture2DDesc& desc) = 0;

        /**
         * @brief Creates a 2D texture from an Image::IImage object.
         * @param desc
         * @return
         */
        virtual View<Texture2D> createTexture2D(const Texture2DImageDesc& desc) = 0;

        /**
         * @brief Destroys a 2D texture
         * @param texture2D The 2D texture to destroy
         */
        virtual void destroyTexture2D(const View<Texture2D>& texture2D) = 0;

        /**
         * @brief Creates a framebuffer layout object
         * @return
         */
        virtual View<FrameBufferLayout> createFrameBufferLayout() = 0;

        /**
         * @brief Destroys a framebuffer layout
         * @param frameBufferLayout The framebuffer layout to destroy
         */
        virtual void destroyFrameBufferLayout(const View<FrameBufferLayout>& frameBufferLayout) = 0;

        /**
         * @brief Creates a framebuffer object
         * @param frameBufferLayout This layout object describes what attachments to create in the framebuffer
         * @return
         */
        virtual View<FrameBuffer> createFrameBuffer(const View<FrameBufferLayout>& frameBufferLayout) = 0;

        /**
         * @brief Destroys a framebuffer
         * @param frameBuffer The framebuffer to destroy
         */
        virtual void destroyFrameBuffer(const View<FrameBuffer>& frameBuffer) = 0;

        virtual View<DepthStencilState> createDepthStencilState(const DepthStencilStateDesc& desc) = 0;

        /**
         * @brief Destroys a depth stencil state
         * @param depthStencilState The depth stencil state to destroy
         */
        virtual void destroyDepthStencilState(const View<DepthStencilState>& depthStencilState) = 0;

        /**
         * @brief Returns the current API state which includes info like current binds, viewport config, framebuffers,
         *        etc. Handy for debugging, this is quite an expensive call to make so don't use this in non debug builds.
         * @return API state
         */
        virtual UP<ApiState> getState() = 0;

        /**
         * @brief Returns a reference to the default framebuffer.
         * @return
         */
        const UP<FrameBuffer>& getDefaultFrameBuffer() const {
            VL_PRECONDITION(m_DefaultFrameBuffer != nullptr, "Default framebuffer is null")

            return m_DefaultFrameBuffer;
        }

        /**
         * @brief Clears the default framebuffer. Equivalent to getDefaultFrameBuffer()->clear(), but provided for
         * convenience.
         */
        void clear() const {
            VL_PRECONDITION(m_DefaultFrameBuffer != nullptr, "Default framebuffer is null")

            m_DefaultFrameBuffer->clear();
        }

        /**
         * @brief Begins rendering to the default framebuffer. Equivalent to getDefaultFrameBuffer()->begin(), but
         * provided for convenience.
         */
        void begin() const {
            VL_PRECONDITION(m_DefaultFrameBuffer != nullptr, "Default framebuffer is null")

            m_DefaultFrameBuffer->begin();
        }

        /**
         * @brief Ends rendering to the default framebuffer. Equivalent to getDefaultFrameBuffer()->end(), but provided
         * for convenience.
         */
        void end() const {
            VL_PRECONDITION(m_DefaultFrameBuffer != nullptr, "Default framebuffer is null")

            m_DefaultFrameBuffer->end();
        }

    protected:
        explicit Context(VL_GRAPHICS_API type);

        static void imGuiSetStyle(VL_IMGUI_STYLE style);

    protected:
        const VL_GRAPHICS_API m_Type;

        // ImGui Stuff
        ImGuiContextDesc m_ImGuiDesc{};

        bool m_ImGuiEnabled = false;
        bool m_ImGuiRendering = false;

        UP<Device> m_Device;
        UP<FrameBuffer> m_DefaultFrameBuffer;

        std::vector<UP<Viewport>> m_Viewports;
        std::vector<UP<ShaderModule>> m_ShaderModules;
        std::vector<UP<Shader>> m_Shaders;
        std::vector<UP<VertexLayout>> m_VertexLayouts;
        std::vector<UP<VertexBuffer>> m_VertexBuffers;
        std::vector<UP<IndexBuffer>> m_IndexBuffers;
        std::vector<UP<MeshBinding>> m_MeshBindings;
        std::vector<UP<ConstantBuffer>> m_ConstantBuffers;
        std::vector<UP<Sampler>> m_Samplers;
        std::vector<UP<Texture2D>> m_Texture2Ds;
        std::vector<UP<FrameBufferLayout>> m_FrameBufferLayouts;
        std::vector<UP<FrameBuffer>> m_FrameBuffers;
        std::vector<UP<DepthStencilState>> m_DepthStencilStates;
    };


}
