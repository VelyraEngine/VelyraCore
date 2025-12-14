#pragma once

#include <VelyraCore/Context/Definitions.hpp>

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

    struct VL_API ImGuiDesc {
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
        virtual bool isVerticalSynchronisationEnabled() const = 0;

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

        virtual void initImGui(const ImGuiDesc& desc);

        virtual void terminateImGui();

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

    protected:
        explicit Context(const VL_GRAPHICS_API type): m_Type(type) {}

        void imGuiSetStyle(VL_IMGUI_STYLE style);

    protected:
        const VL_GRAPHICS_API m_Type;

        bool m_ImGuiEnabled = false;
        bool m_ImPlotEnabled = false;
        bool m_ImGuiRendering = false;
    };


}