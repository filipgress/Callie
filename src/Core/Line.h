#pragma once 

#include <glm/glm.hpp>

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>

namespace cl{
    class Line{
        private:
            VertexArray  m_VAO;
            VertexBuffer m_VBO; 
            glm::vec4    m_Color;

        public:
            Line(glm::vec3 a, glm::vec3 b, glm::vec4 color);

            void Draw();
            glm::vec4 GetColor() const { return m_Color; }
    };
}