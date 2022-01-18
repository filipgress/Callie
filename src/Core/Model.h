#pragma once

#include <glm/glm.hpp>
#include <Core/Mesh.h>

namespace cl{
    class Transform{
        private:
            glm::vec3 m_Translate; 
            glm::vec3 m_Rotate; 
            glm::vec3 m_Scale; 

        public:
            Transform(glm::vec3 Translate): m_Translate(Translate) {}
    };

    class Model{
        private:
            Transform m_Transform;
            Mesh m_Mesh;

        public:
    };
}