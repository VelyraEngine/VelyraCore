#pragma once

#include "GLTranslations.hpp"

namespace Velyra::Core {

    class GLBuffer {
    public:
        GLBuffer(GLenum target, Size size, const void* data, VL_BUFFER_USAGE usage);

        GLBuffer(const std::string& name, GLenum target, Size size, const void* data, VL_BUFFER_USAGE usage);

        ~GLBuffer();

        void bind() const;

        void bindShaderResource(U32 slot) const;

        void setData(U64 offset, const void* data, U64 size);

        void copyFrom(const GLBuffer& other);

        [[nodiscard]] std::vector<UByte> getData() const;

        [[nodiscard]] GLuint getBufferID() const {
            return m_BufferID;
        }

        [[nodiscard]] Size getSize() const {
            return m_Size;
        }

        [[nodiscard]] VL_BUFFER_USAGE getUsage() const {
            return m_Usage;
        }

        [[nodiscard]] const std::string& getName() const {
            return m_Name;
        }

        [[nodiscard]] GLenum getTarget() const {
            return m_Target;
        }

    private:
        const Utils::LogPtr m_Logger;
        const std::string m_Name;
        const Size m_Size = 0;
        const VL_BUFFER_USAGE m_Usage = VL_BUFFER_USAGE_DEFAULT;

        GLuint m_BufferID = 0;
        GLenum m_Target = 0;
    };
}