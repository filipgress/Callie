#include <clpch.h>

#include <Core/Camera.h>

namespace cl{
    Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float near, float far)
        : m_Position(position), m_Direction(target), m_Up(up), m_Near(near), m_Far(far), m_OriginalUp(glm::vec3(0.0f, 1.0f, 0.0f)) {}


    glm::mat4 Camera::GetView() const{
        return glm::lookAt(m_Position, m_Direction, m_Up);
    }

    void Camera::RotateAroundTarget(glm::vec3 target, float deltaX, float deltaY){
        glm::mat4 M = glm::mat4(1);
        M = glm::rotate(M, deltaX, glm::vec3(0, 1, 0));
        M = glm::rotate(M, deltaY, glm::vec3(1, 0, 0));

        m_Direction = target;
        glm::vec3 T = glm::vec3(0, 0, glm::distance(m_Direction, m_Position));
        T = glm::vec3(M * glm::vec4(T, 0.0));
        m_Position = m_Direction + T;
        m_Up = glm::vec3(M * glm::vec4(m_OriginalUp, 1.0f));
    }

    void Camera::RotateAroundOrigin(float deltaX, float deltaY){
        RotateAroundTarget(glm::vec3(0.0f, 0.0f, 0.0f), deltaX, deltaY);
    }

    void Camera::RotateAroundDefault(float deltaX, float deltaY){
        RotateAroundTarget(m_Direction, deltaX, deltaY);
    }

    void Camera::Zoom(float delta){
        float distance = glm::length(m_Direction - m_Position);
        m_Position = m_Direction + (m_Position - m_Direction) * (delta + 1);
    }
}