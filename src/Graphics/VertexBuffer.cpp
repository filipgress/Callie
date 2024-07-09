#include <clpch.h>

#include <Graphics/VertexBuffer.h>

namespace cl{
    VertexBuffer::VertexBuffer(const void* data, unsigned int size){
        GLCall(glGenBuffers(1, &m_VBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer::VertexBuffer(glm::vec3 point1, glm::vec3 point2){
        glm::vec3 data[] = {point1, point2};

        GLCall(glGenBuffers(1, &m_VBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 2*sizeof(glm::vec3), data, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer(){
        GLCall(glDeleteBuffers(1, &m_VBO));
    }

    void VertexBuffer::Bind() const{
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    }

    void VertexBuffer::Unbind() const{
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}