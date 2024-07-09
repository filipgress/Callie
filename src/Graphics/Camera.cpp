#include <clpch.h>

#include <GL/glew.h>
#include <Graphics/Camera.h>

namespace cl{
    Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float near, float far)
        : m_Position(position), m_Direction(target), m_Near(near), m_Far(far), 
          m_Projection(CameraMode::Perspective), m_Wireframe(false), changeX(0), changeY(0),
          m_Rotate(glm::mat4(1.0f)) {
            m_Up = glm::normalize(up);
            m_OriginalUp = m_Up;

            m_Right = glm::normalize(glm::cross(m_Position - m_Direction, m_Up));
            m_OriginalRight = m_Right;
        }

    void Camera::RotateAroundTarget(glm::vec3 target, float deltaX, float deltaY){
        changeX += deltaX;
        changeY += deltaY;

        glm::mat4 M = glm::mat4(1.0f);
        M = glm::rotate(M, -changeX*5, glm::vec3(0, 1, 0));
        M = glm::rotate(M, -changeY*5, glm::vec3(1, 0, 0));
        m_Rotate = glm::inverse(M);

        m_Direction = target;
        glm::vec3 T = glm::vec3(0, 0, glm::distance(m_Direction, m_Position));
        T = glm::vec3(M * glm::vec4(T, 0.0));
        m_Position = m_Direction + T;

        m_Up = glm::normalize(glm::vec3(M * glm::vec4(m_OriginalUp, 1.0f)));
        m_Right = glm::normalize(glm::vec3(M * glm::vec4(m_OriginalRight, 1.0f)));
    }

    void Camera::RotateAroundOrigin(float deltaX, float deltaY){
        RotateAroundTarget(glm::vec3(0.0f, 0.0f, 0.0f), deltaX, deltaY);
    }

    void Camera::RotateAroundDefault(float deltaX, float deltaY){
        RotateAroundTarget(m_Direction, deltaX, deltaY);
    }

    void Camera::Zoom(float delta){
        float distance = GetDistance()*10;

        if (distance < 0.0f) distance = 0.0f;
        else if (distance > 3.5f) distance = 3.5f;
        m_Position = m_Direction + (m_Position - m_Direction) * (1 + delta * distance);
    }

    void Camera::Translate(float deltaX, float deltaY){
        deltaX *=5.0f;
        deltaY *=5.0f;

        m_Position  += m_Right * deltaX;
        m_Position  += m_Up * deltaY;

        m_Direction += m_Right * deltaX;
        m_Direction += m_Up * deltaY;
    }

    glm::mat4 Camera::GetView() const{
        return glm::lookAt(m_Position, m_Direction, m_Up);
    }

    float Camera::GetDistance() const{
        return glm::length(m_Direction - m_Position);
    }

    void Camera::ToggleProjection(){
        if (m_Projection == CameraMode::Perspective){
            m_Projection = CameraMode::Orthografic;
        } else{
            m_Projection = CameraMode::Perspective;
        }
    }
}