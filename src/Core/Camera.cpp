#include <clpch.h>

#include <Core/Camera.h>

namespace cl{
    Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec3 right, float near, float far)
        : m_Position(position), m_Direction(target), m_Up(up), m_Right(right), m_Near(near), m_Far(far), 
          m_OriginalUp(m_Up), m_OriginalRight(m_Right), m_Rotate(glm::mat4(1.0f)) {}

    glm::mat4 Camera::GetView() const{
        return glm::lookAt(m_Position, m_Direction, m_Up);
    }

    void Camera::RotateAroundTarget(glm::vec3 target, float deltaX, float deltaY){
        glm::mat4 M = glm::mat4(1.0f);
        M = glm::rotate(M, -deltaX, glm::vec3(0, 1, 0));
        M = glm::rotate(M, -deltaY, glm::vec3(1, 0, 0));
        m_Rotate = glm::inverse(M);

        m_Direction = target;
        glm::vec3 T = glm::vec3(0, 0, glm::distance(m_Direction, m_Position));
        T = glm::vec3(M * glm::vec4(T, 0.0));
        m_Position = m_Direction + T;
        m_Up = glm::normalize(glm::vec3(M * glm::vec4(m_OriginalUp, 1.0f)));
        m_Right = glm::normalize(glm::vec3(M* glm::vec4(m_OriginalRight, 1.0f)));
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

    void Camera::Translate(float deltaX, float deltaY){
        m_Position  -= m_Right * deltaX;
        m_Position  += m_Up * deltaY;
        m_Direction -= m_Right * deltaX;
        m_Direction += m_Up * deltaY;
    }
}