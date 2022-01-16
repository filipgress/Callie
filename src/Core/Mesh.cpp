#include <clpch.h>

#include <Core/Mesh.h>

namespace cl{
    Mesh::Mesh(const void* data, unsigned int* indeces, unsigned int sizeOfData, unsigned int numOfIndeces)
        : m_IBO(indeces, numOfIndeces), m_VBO(data, sizeOfData)
    {
        m_Layout.Push<float>(3);
        m_VAO.AddBuffer(m_VBO, m_Layout);

        m_VAO.Unbind();
        m_VBO.Unbind();
        m_IBO.Unbind();
    }

    // void Mesh::RenderMesh() const{
    //     m_VAO.Bind();
    //     m_IBO.Bind();
    //         glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, 0);
    //     m_VAO.Unbind();
    //     m_IBO.Unbind();

    // }
}