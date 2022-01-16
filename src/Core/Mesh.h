#pragma once

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>

namespace cl{
    class Mesh{
        private:
            VertexArray m_VAO;
            IndexBuffer m_IBO;
            VertexBuffer m_VBO;
            VertexBufferLayout m_Layout;

        public:
            Mesh(const void* data, unsigned int* indeces, unsigned int sizeOfData, unsigned int numOfIndeces);
            void CreateMesh() const;
            void ClearMesh() const;
    };
}