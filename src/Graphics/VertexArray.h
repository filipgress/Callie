#pragma once

#include <GL/glew.h>

#include <Graphics/VertexBuffer.h>
#include <Graphics/VertexBufferLayout.h>

namespace cl{
    class VertexArray{
        private:
            GLuint m_VAO;

        public:
            VertexArray();
            ~VertexArray();

            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            void Bind() const;
            void Unbind() const;
    };
}