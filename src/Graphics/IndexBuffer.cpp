#include <clpch.h>

#include <Graphics/IndexBuffer.h>

namespace cl{
    IndexBuffer::IndexBuffer(const GLuint* data, unsigned int count)
        : m_Count(count)     
    {
        GLCall(glGenBuffers(1, &m_IBO));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count*sizeof(GLuint), data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer(){
        GLCall(glDeleteBuffers(1, &m_IBO));
    }

    void IndexBuffer::Bind() const{
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
    }

    void IndexBuffer::Unbind() const{
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}