#include <clpch.h>

#include <Graphics/VertexArray.h>

namespace cl{
    VertexArray::VertexArray(){
        GLCall(glGenVertexArrays(1, &m_VAO));
    }

    VertexArray::~VertexArray(){
        GLCall(glDeleteVertexArrays(1, &m_VAO));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
        Bind();
        vb.Bind();

        const auto& elements = layout.GetElements();
        for(unsigned int i = 0; i < elements.size(); i++){
            const auto& element = elements[i];

            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, 
                layout.GetStride(), reinterpret_cast<const void*>(element.offset)));
        }
    }

    void VertexArray::Bind() const{
        GLCall(glBindVertexArray(m_VAO));
    }

    void VertexArray::Unbind() const{
        GLCall(glBindVertexArray(0));
    }
}