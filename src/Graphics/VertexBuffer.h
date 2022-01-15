#pragma once

namespace cl{
    class VertexBuffer{
        private:
            GLuint m_VBO;

        public:
            VertexBuffer(const void* data, unsigned int size);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;
    };
}