#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Line.h>

namespace cl{
    class Camera{
        private:
            glm::vec3 m_Position;
            glm::vec3 m_Direction;
            glm::vec3 m_Up;
            glm::vec3 m_Right;
            glm::vec3 m_OriginalUp;
            glm::vec3 m_OriginalRight;
            glm::mat4 m_Rotate;

            float m_Near, m_Far;

        public:
            Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), 
                glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), 
                float near=0.1f, float far=1000.0f);

            glm::mat4 GetView() const;
            glm::mat4 GetRotate() const { return m_Rotate; }
        
            void Translate(float deltaX, float DeltaY);
            void RotateAroundTarget(glm::vec3 target, float deltaX, float deltaY);
            void RotateAroundOrigin(float deltaX, float deltaY);
            void RotateAroundDefault(float deltaX, float deltaY);
            void Zoom(float delta);
    };
}