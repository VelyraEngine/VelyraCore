#pragma once

#include <VelyraCore/Context/Definitions.hpp>
#include <VelyraCore/Context/Viewport.hpp>
#include <VelyraCore/Context/ShaderModule.hpp>
#include <VelyraCore/Context/Shader.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/IndexBuffer.hpp>

#include "Device.hpp"

namespace Velyra::Core {

    struct VL_API ContextDesc {
        VL_GRAPHICS_API api = VL_API_BEST;
        U8 depthBits        = 24;
        U8 stencilBits      = 8;
        U8 redBits          = 8;
        U8 blueBits         = 8;
        U8 greenBits        = 8;
        U8 alphaBits        = 8;
        bool enableDepthTest   = false;
        bool enableStencilTest = false;
    };

    struct VL_API ImGuiContextDesc {
        VL_IMGUI_STYLE style = VL_IMGUI_STYLE_DEFAULT;
        bool useImPlot = true;
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
            return m_ImPlotEnabled;
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
         * @brief Creates a shader module and compiles the code from a std::string.
         * @param desc
         * @return
         */
        virtual SP<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) = 0;

        /**
         * @brief Creates a shader module and compiles the code from a file.
         * @param desc
         * @return
         */
        virtual SP<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) = 0;

        /**
         * @brief Creates a shader given a valid vertex and fragment shader module.
         *        (with extra optional shaders)
         * @param desc
         * @return
         */
        virtual SP<Shader> createShader(const ShaderDesc& desc) = 0;

        /**
         * @brief Creates a vertex layout object, used to configure vertex buffers.
         * @return
         */
        virtual SP<VertexLayout> createVertexLayout() = 0;

        /**
         * @brief Creates a vertex buffer given a valid description.
         * @param desc Description of the vertex buffer
         * @return
         */
        virtual SP<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) = 0;

        /**
         * @brief Creates an index buffer given a valid description.
         * @param desc Description of the index buffer
         * @return
         */
        virtual SP<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) = 0;

    protected:
        explicit Context(const VL_GRAPHICS_API type): m_Type(type) {}

        void imGuiSetStyle(VL_IMGUI_STYLE style);

    protected:
        const VL_GRAPHICS_API m_Type;

        bool m_ImGuiEnabled = false;
        bool m_ImPlotEnabled = false;
        bool m_ImGuiRendering = false;

        UP<Device> m_Device = nullptr;

        std::vector<SP<ShaderModule>> m_ShaderModules;
        std::vector<SP<Shader>> m_Shaders;
        std::vector<SP<VertexLayout>> m_VertexLayouts;
        std::vector<SP<VertexBuffer>> m_VertexBuffers;
        std::vector<SP<IndexBuffer>> m_IndexBuffers;
    };


}
