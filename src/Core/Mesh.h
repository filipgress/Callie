#pragma once

#include <glm/glm.hpp>
#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>

namespace cl{
    struct Vertex{
        private:
            glm::vec3 m_Position;
        
        public:
            Vertex(glm::vec3 position): m_Position(position) {}
    };

    class Mesh{
        private:
            VertexArray m_VAO;
            IndexBuffer m_IBO;
            VertexBuffer m_VBO;
            VertexBufferLayout m_Layout;

        public:
            Mesh(Vertex* data, unsigned int* indeces, unsigned int sizeOfData, unsigned int numOfIndeces);

            void Bind();
            void Unbind();

            void Draw();

            bool static s_Viewport;
    };

}