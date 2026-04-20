#include "TextureProcedure.hpp"

#include <VelyraImage/ImageFactory.hpp>

namespace Velyra::SandBox {

    void TextureProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        createTextureParametersBuffer(context);

        loadImage(m_AwesomeFace, "./Resources/Images/awesomeface.png");
        loadImage(m_Instagram, "./Resources/Images/insta.png");
        loadImage(m_VelyraLogo, "./Resources/Images/Velyra.png");

        // Set by default to the awesome face, but you can change it in the ImGui window
        Core::Texture2DImageDesc texDesc;
        texDesc.image = createView(m_AwesomeFace);
        texDesc.usage = VL_BUFFER_USAGE_DYNAMIC;
        m_Texture = context->createTexture2D(texDesc);

        Core::Texture2DImageDesc tex2Desc;
        tex2Desc.image = createView(m_Instagram);
        tex2Desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        m_Texture2 = context->createTexture2D(tex2Desc);
    }

    void TextureProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ParametersBuffer->bindShaderResource(4);
        m_Texture->bindShaderResource(0);
        m_Texture2->bindShaderResource(1);
    }

    void TextureProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        ImGui::Begin("Texture");
        imguiTextureParameters();
        ImGui::End();
    }

    void TextureProcedure::createTextureParametersBuffer(const UP<Core::Context> &context) {
        Core::ConstantBufferDesc desc;
        desc.data = &m_Parameters;
        desc.size = sizeof(TextureParameters);
        desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        desc.shaderStage = VL_SHADER_FRAGMENT;
        desc.name = "TextureParameters";
        m_ParametersBuffer = context->createConstantBuffer(desc);
    }

    void TextureProcedure::loadImage(UP<Image::IImage> &image, const fs::path &filePath) {
        using namespace Image;

        ImageLoadDesc desc;
        desc.fileName = filePath;
        desc.flipOnLoad = true;
        image = ImageFactory::createImage(desc);
    }

    void TextureProcedure::imguiTextureParameters() {
        if (ImGui::SliderFloat("Lerp Factor", &m_Parameters.lerpFactor, 0.0f, 1.0f)){
            m_ParametersBuffer->setData(0, &m_Parameters, sizeof(TextureParameters));
        }
        if (ImGui::SliderFloat("Scale", &m_Parameters.scale, 0.0f, 50.0f)){
            m_ParametersBuffer->setData(0, &m_Parameters, sizeof(TextureParameters));
        }
    }
}
