#pragma once

#include "../DataTypes.hpp"
#include "../IProcedure.hpp"

namespace Velyra::SandBox {

    struct TextureParameters {
        float lerpFactor = 0.0f;
        float scale = 1.0f;
        float padding[2] = {0.0f, 0.0f};
    };

    class TextureProcedure: public IProcedure {
    public:
        TextureProcedure() = default;

        ~TextureProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context>& context, const UP<Core::Window>& window) override;

    private:

        void createTextureParametersBuffer(const UP<Core::Context>& context);

        void loadImage(UP<Image::IImage>& image, const fs::path& filePath);

        void imguiTextureParameters();

    private:
        TextureParameters m_Parameters;

        View<Core::Texture2D> m_Texture = nullptr;
        View<Core::Texture2D> m_Texture2 = nullptr;
        View<Core::ConstantBuffer> m_ParametersBuffer = nullptr;

        UP<Image::IImage> m_AwesomeFace = nullptr;
        UP<Image::IImage> m_Instagram = nullptr;
        UP<Image::IImage> m_VelyraLogo = nullptr;
    };

    static const std::vector s_TextureProcedures = {
        Sampler
    };

    VL_DECLARE_PROCEDURE_FACTORY(TextureProcedure, Texture2D, s_TextureProcedures)
}