#include "ModelViewProjectionProcedure.hpp"

namespace Velyra::SandBox {

    ModelViewProjectionProcedure::ModelViewProjectionProcedure():
    IProcedure(){

    }

    void ModelViewProjectionProcedure::onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram = loadShader(context, "MVP", "MVP");
        const auto mesh = createCube();
        m_MeshBinding = loadMesh(mesh, context);

        const glm::mat4 zero = glm::mat4(1.0f);
        Core::ConstantBufferDesc desc;
        desc.name = "CameraData";
        desc.data = &zero;
        desc.size = sizeof( glm::mat4);
        desc.usage = VL_BUFFER_USAGE_DYNAMIC;
        desc.shaderStage = VL_SHADER_VERTEX;
        m_TransformBuffer = context->createConstantBuffer(desc);
    }

    void ModelViewProjectionProcedure::onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) {
        m_ShaderProgram.shaderProgram->bind();
        m_TransformBuffer->bindShaderResource(0);
        m_MeshBinding->draw();
    }

    void ModelViewProjectionProcedure::onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) {

    }

}
