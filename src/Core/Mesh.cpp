#include <clpch.h>

#include <Core/Mesh.h>

namespace cl{
    Mesh::Mesh(Vertex* data, unsigned int* indeces, unsigned int sizeOfData, unsigned int numOfIndeces)
        : m_IBO(indeces, numOfIndeces), m_VBO(data, sizeOfData), m_Mode(GL_TRIANGLES) {
        m_Layout.Push<float>(3);
        m_VAO.AddBuffer(m_VBO, m_Layout);
    }

    void Mesh::Bind(){
        m_VAO.Bind();
        m_IBO.Bind();
    }

    void Mesh::Unbind(){
        m_VAO.Unbind();
        m_IBO.Unbind();
    }

    void Mesh::Draw(){
        Bind();
        GLCall(glDrawElements(m_Mode, m_IBO.GetCount(), GL_UNSIGNED_INT, 0));
    }
}