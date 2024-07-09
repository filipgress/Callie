#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace cl{
    class VertexBuffer{
        private:
            GLuint m_VBO;

        public:
            VertexBuffer(const void* data, unsigned int size);
            VertexBuffer(glm::vec3 point1, glm::vec3 point2);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;
    };
}
