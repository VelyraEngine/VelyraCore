#pragma once

#include <VelyraCore/Context/ApiState.hpp>

namespace Velyra::Core {

    class GLState : public ApiState {
    public:
        GLState();

        ~GLState() override = default;

        bool isEqual(const ApiState &other) const override;

        void log() const override;

    private:


    void capture();


    private:
        const Utils::LogPtr m_Logger;

        GLint m_Viewport[4]{};
        GLboolean m_ColorMask[4]{};

        GLboolean m_ScissorEnabled = GL_FALSE;
        GLint m_ScissorBox[4]{};

        GLboolean m_DepthTestEnabled = GL_FALSE;
        GLint m_DepthFunc = GL_LESS;
        GLboolean m_DepthMask = GL_TRUE;
        GLboolean m_BlendEnabled = GL_FALSE;
        GLboolean m_CullFaceEnabled = GL_FALSE;
        GLint m_FrontFace = GL_FRONT;
        GLint m_CullFace = GL_BACK;

        GLint m_ArrayBuffer = 0;
        GLint m_ElementArrayBuffer = 0;
        GLint m_UniformBuffer = 0;
        GLint m_DrawFBO = 0;
        GLint m_ReadFBO = 0;

        GLint m_Vao = 0;
        GLint m_ShaderProgram = 0;

        GLint m_ActiveTexture = 0;
        GLint m_BoundTextures[32]{};
        GLint m_UniformBindings[32]{};

    };

}
