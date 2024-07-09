#pragma once

#include <GL/glew.h>

namespace cl{
    class IndexBuffer{
        private:
            GLuint m_IBO;
            unsigned int m_Count;

        public:
            IndexBuffer(const GLuint* data, unsigned int count);
            ~IndexBuffer();

            void Bind() const;
            void Unbind() const;

            inline unsigned int GetCount() const { return m_Count; }
    };
}