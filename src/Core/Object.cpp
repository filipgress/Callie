#include <clpch.h>

#include <Core/Object.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Application.h>

namespace cl{
    Line::Line(glm::vec3 a, glm::vec3 b, glm::vec4 color)
        : m_VBO(a, b), m_Color(color){
        VertexBufferLayout layout;

        layout.Push<float>(3);
        m_VAO.AddBuffer(m_VBO, layout);
    }

    void Line::Draw() const{
        m_VAO.Bind();
        GLCall(glDrawArrays(GL_LINES, 0, 2));
        m_VAO.Unbind();
    }

    Mesh::Mesh(std::vector<Vertex> verteces, std::vector<unsigned int> indeces)
        : m_VBO(verteces.data(), verteces.size()*sizeof(Vertex)), m_IBO(indeces.data(), indeces.size()) {
        m_Layout.Push<float>(3);
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

    Object::Object(const std::string name, std::vector<Vertex> verteces,
        std::vector<uint32_t> indeces, bool visible)
        : m_Filepath(name), m_Mesh(verteces, indeces), m_Visible(visible), m_Pos(glm::vec3(0.0f)),
          m_Rot(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Color(glm::vec4(1.0f)) {}

    void Object::Bind(){
        m_Mesh.Bind();
    }

    void Object::Unbind(){
        m_Mesh.Unbind();
    }
    
    const std::string Object::GetName() const{
        return m_Filepath.substr(m_Filepath.find_last_of("/\\") + 1);
    }

    glm::mat4 Object::GetModel() const{
        glm::mat4 model(1.0f);
        model = glm::translate(model, m_Pos);
        model = glm::rotate(model, m_Rot.x, glm::vec3(1,0,0));
        model = glm::rotate(model, m_Rot.y, glm::vec3(0,1,0));
        model = glm::rotate(model, m_Rot.z, glm::vec3(1,0,0));
        model = glm::scale(model, m_Scale);

        return model;
    }
}