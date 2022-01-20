#include <clpch.h>

#include <Core/Line.h>

namespace cl{
    Line::Line(glm::vec3 a, glm::vec3 b, glm::vec4 color)
        : m_VBO(VertexBuffer(a, b)), m_Color(color){ 
        VertexBufferLayout layout;

        layout.Push<float>(3);
        m_VAO.AddBuffer(m_VBO, layout);
    }

    void Line::Draw(){
        m_VAO.Bind();
        GLCall(glDrawArrays(GL_LINES, 0, 2));
        m_VAO.Unbind();
    }
}