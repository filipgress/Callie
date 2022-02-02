#include <clpch.h>

#include <Graphics/FrameBuffer.h>

namespace cl{
    void FrameBuffer::CreateBuffers(int32_t width, int32_t height){
        m_Width = width;
        m_Height = height;

        if (m_FBO)
            DeleteBuffers();

        GLCall(glGenFramebuffers(1, &m_FBO));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_TexID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0));

        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthID));
        GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthID, 0));

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
        GLCall(glDrawBuffers(m_TexID, buffers));

        Unbind();
    }
    void FrameBuffer::DeleteBuffers(){
        if (m_FBO){
            GLCall(glDeleteFramebuffers(GL_FRAMEBUFFER, &m_FBO));
            GLCall(glDeleteTextures(1, &m_TexID));
            GLCall(glDeleteTextures(1, &m_DepthID));
            m_TexID = 0;
            m_DepthID = 0;
        }
    }
    void FrameBuffer::Bind(){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        GLCall(glViewport(0, 0, m_Width, m_Height));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void FrameBuffer::Unbind(){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}