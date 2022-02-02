#pragma once

#include <GL/glew.h>

namespace cl{
    class FrameBuffer{
        private:
            GLuint m_FBO = 0;
            uint32_t m_Width, m_Height;
            uint32_t m_TexID;
            uint32_t m_DepthID;

    public:
        void CreateBuffers(int32_t width, int32_t height);
        void DeleteBuffers();
        void Bind();
        void Unbind();
        inline uint32_t GetTexture() const { return m_TexID; }
    };
}