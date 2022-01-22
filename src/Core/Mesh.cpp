#include <clpch.h>

#include <Core/Mesh.h>

namespace cl{
    bool Mesh::s_Viewport = false;
    Mesh::Mesh(Vertex* data, unsigned int* indeces, unsigned int sizeOfData, unsigned int numOfIndeces)
        : m_IBO(indeces, numOfIndeces), m_VBO(data, sizeOfData) {
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
        if (!s_Viewport){
            GLCall(glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, 0));
        }else
            GLCall(glDrawElements(GL_LINES, m_IBO.GetCount(), GL_UNSIGNED_INT, 0));
    }
}