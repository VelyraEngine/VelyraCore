#pragma once

#include <glad/glad.h>

// OpenGL ES has no DSA, so objects must be bound to be modified. These guards restore the caller's bindings afterwards.
namespace Velyra::Core::GLES {

    class Texture2DBindingGuard {
    public:
        explicit Texture2DBindingGuard(const GLuint texture) {
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_Previous);
            m_Changed = static_cast<GLuint>(m_Previous) != texture;
            if (m_Changed) {
                glBindTexture(GL_TEXTURE_2D, texture);
            }
        }

        ~Texture2DBindingGuard() {
            if (m_Changed) {
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(m_Previous));
            }
        }

        Texture2DBindingGuard(const Texture2DBindingGuard&) = delete;
        Texture2DBindingGuard& operator=(const Texture2DBindingGuard&) = delete;

    private:
        GLint m_Previous = 0;
        bool m_Changed = false;
    };

    class FramebufferBindingGuard {
    public:
        // target must be GL_DRAW_FRAMEBUFFER or GL_READ_FRAMEBUFFER
        FramebufferBindingGuard(const GLenum target, const GLuint framebuffer): m_Target(target) {
            const GLenum bindingQuery = target == GL_READ_FRAMEBUFFER ? GL_READ_FRAMEBUFFER_BINDING : GL_DRAW_FRAMEBUFFER_BINDING;
            glGetIntegerv(bindingQuery, &m_Previous);
            m_Changed = static_cast<GLuint>(m_Previous) != framebuffer;
            if (m_Changed) {
                glBindFramebuffer(m_Target, framebuffer);
            }
        }

        ~FramebufferBindingGuard() {
            if (m_Changed) {
                glBindFramebuffer(m_Target, static_cast<GLuint>(m_Previous));
            }
        }

        FramebufferBindingGuard(const FramebufferBindingGuard&) = delete;
        FramebufferBindingGuard& operator=(const FramebufferBindingGuard&) = delete;

    private:
        GLenum m_Target;
        GLint m_Previous = 0;
        bool m_Changed = false;
    };

    class PixelStoreGuard {
    public:
        // pname must be a glPixelStorei parameter that is also valid for glGetIntegerv (e.g. GL_UNPACK_ALIGNMENT)
        PixelStoreGuard(const GLenum pname, const GLint value): m_Name(pname) {
            glGetIntegerv(pname, &m_Previous);
            m_Changed = m_Previous != value;
            if (m_Changed) {
                glPixelStorei(m_Name, value);
            }
        }

        ~PixelStoreGuard() {
            if (m_Changed) {
                glPixelStorei(m_Name, m_Previous);
            }
        }

        PixelStoreGuard(const PixelStoreGuard&) = delete;
        PixelStoreGuard& operator=(const PixelStoreGuard&) = delete;

    private:
        GLenum m_Name;
        GLint m_Previous = 0;
        bool m_Changed = false;
    };

}
