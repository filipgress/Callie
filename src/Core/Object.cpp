#include <clpch.h>

#include <Core/Object.h>
#include <glm/gtc/matrix_transform.hpp>

namespace cl{
    Object::Object(const std::string name, Vertex* verteces, unsigned int size,
        unsigned int* indeces, unsigned int count, bool visible)
        : m_Name(name), m_IBO(indeces, count), m_VBO(verteces, size), m_Visible(visible), m_Pos(glm::vec3(0.0f)),
          m_Rot(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Color(glm::vec4(1.0f)) {
            m_Layout.Push<float>(3);
            m_VAO.AddBuffer(m_VBO, m_Layout);
        }

    void Object::Bind(){
        m_VAO.Bind();
        m_IBO.Bind();
    }

    void Object::Unbind(){
        m_VAO.Unbind();
        m_IBO.Unbind();
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