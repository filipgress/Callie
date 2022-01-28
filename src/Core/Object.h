#pragma once

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/VertexBufferLayout.h>
#include <Graphics/IndexBuffer.h>

namespace cl{
    struct Vertex{
        glm::vec3 m_Position;

        public:
            Vertex(glm::vec3 position): m_Position(position) {}
    };

    class Object{
        private:
            VertexArray m_VAO;
            VertexBuffer m_VBO;
            IndexBuffer m_IBO;
            VertexBufferLayout m_Layout;

            glm::vec3 m_Pos;
            glm::vec3 m_Rot;
            glm::vec3 m_Scale;
            glm::vec4 m_Color;

            bool m_Visible;

        public:
            Object(Vertex* verteces, unsigned int size, unsigned int* indeces,
                unsigned int count, bool visible = true);

            inline void SetPos(glm::vec3 position) { m_Pos   = position; }
            inline void SetRot(glm::vec3 rotation) { m_Rot   = rotation; }
            inline void SetScale(glm::vec3 scale)  { m_Scale = scale; }
            inline void SetColor(glm::vec4 color)  { m_Color = color; }
            inline glm::vec4 GetColor() const { return m_Color; }

            inline bool IsVisible() const { return m_Visible; }
            inline void Hide() { m_Visible = false; }
            inline void Show() { m_Visible = true; }

            void Bind();
            void Unbind();

            inline unsigned int GetVerteces() const { return m_IBO.GetCount(); }
            glm::mat4 GetModel() const;
    };
}