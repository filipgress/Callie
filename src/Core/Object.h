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
            const std::string m_Name;
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
            Object(const std::string name, Vertex* verteces, unsigned int size, unsigned int* indeces,
                unsigned int count, bool visible = true);

            inline const std::string& GetName() const { return m_Name; }
            inline glm::vec4& GetColor() { return m_Color; }
            inline glm::vec3& GetPos() { return m_Pos; }
            inline glm::vec3& GetRot() { return m_Rot; }
            inline glm::vec3& GetScale() { return m_Scale; }

            inline bool IsVisible() const { return m_Visible; }
            inline void Hide() { m_Visible = false; }
            inline void Show() { m_Visible = true; }

            void Bind();
            void Unbind();

            inline unsigned int GetVerteces() const { return m_IBO.GetCount(); }
            glm::mat4 GetModel() const;
    };
}