#pragma once

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/VertexBufferLayout.h>
#include <Graphics/IndexBuffer.h>

namespace cl{
    struct Vertex{
        glm::vec3 m_Position;
        glm::vec3 m_Normal;

        public:
            Vertex(glm::vec3 position, glm::vec3 normal): m_Position(position), m_Normal(normal) {}
    };

    class Line{
        private:
            VertexArray m_VAO;
            VertexBuffer m_VBO;
            glm::vec4 m_Color;

        public:
            Line(glm::vec3 a, glm::vec3 b, glm::vec4 color);

            void Draw() const;
            glm::vec4 GetColor() const { return m_Color; }
    };

    class Mesh{
        private:
            VertexArray m_VAO;
            VertexBuffer m_VBO;
            IndexBuffer m_IBO;
            VertexBufferLayout m_Layout;

        public:
            Mesh(std::vector<Vertex> verteces, std::vector<uint32_t> indeces);
            
            void Bind();
            void Unbind();

            unsigned int GetCount() const { return m_IBO.GetCount(); }
    };

    class Object{
        private:
            const std::string m_Filepath;

            Mesh m_Mesh;
            glm::vec3 m_Pos;
            glm::vec3 m_Rot;
            glm::vec3 m_Scale;
            glm::vec4 m_Color;

            bool m_Visible;

        public:
            Object(const std::string name, std::vector<Vertex> verteces,
                std::vector<unsigned int> indeces, bool visible = true);

            const std::string GetName() const;
            inline const std::string& GetFilepath() const { return m_Filepath; }
            inline glm::vec4& GetColor() { return m_Color; }
            inline glm::vec3& GetPos() { return m_Pos; }
            inline glm::vec3& GetRot() { return m_Rot; }
            inline glm::vec3& GetScale() { return m_Scale; }

            unsigned int GetVerteces() const { return m_Mesh.GetCount(); }
            glm::mat4 GetModel() const;

            inline bool IsVisible() const { return m_Visible; }
            inline void Hide() { m_Visible = false; }
            inline void Show() { m_Visible = true; }

            void Bind();
            void Unbind();
    };
}