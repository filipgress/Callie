#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cl{
    enum class CameraMode { Perspective = 0, Orthografic = 1 };
    class Camera{

        private:
            glm::vec3 m_Position;
            glm::vec3 m_Direction;

            glm::vec3 m_Up;
            glm::vec3 m_OriginalUp;
            glm::vec3 m_Right;
            glm::vec3 m_OriginalRight;

            float m_Near, m_Far;
            CameraMode m_Projection;
            bool m_Wireframe;

        public:
            Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float near=0.1f, float far=1000.0f);

            glm::mat4 GetView() const;

            inline float GetNear() const { return m_Near; }
            inline float GetFar() const { return m_Far; }

            inline void SetProjection(CameraMode mode) { m_Projection  = mode; }
            inline CameraMode GetProjection() const { return m_Projection; }

            inline void EnableWireframe() { m_Wireframe = true; }
            inline void DisableWireframe() { m_Wireframe = false; }
            inline bool GetMode() const { return  m_Wireframe; }
        
            void Translate(float deltaX, float DeltaY);
            void RotateAroundTarget(glm::vec3 target, float deltaX, float deltaY);
            void RotateAroundOrigin(float deltaX, float deltaY);
            void RotateAroundDefault(float deltaX, float deltaY);
            void Zoom(float delta);
    };
}