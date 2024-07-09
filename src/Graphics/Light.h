#pragma once

#include <glm/glm.hpp>

namespace cl{
    class Light{
        private:
            // ambient light
            glm::vec4 m_Color;
            float m_AmbientIntensity;

            // diffuse light
            glm::vec3 m_Direction;
            float m_DiffuseIntensity;

        public:
            Light(): m_Color(glm::vec4(1.0f)), m_AmbientIntensity(0.2f),
                m_Direction(glm::vec3(1.0f, 1.0f, 0.0f)), m_DiffuseIntensity(0.5f) {}

            inline glm::vec4& GetColor() { return m_Color; }
            inline float& GetIntensity() { return m_AmbientIntensity; }
            inline glm::vec3& GetDirection() { return m_Direction; }
            inline float& GetDiffuseIntensity() { return m_DiffuseIntensity; }

            inline void SetColor(glm::vec4 color) { m_Color = color; }
            inline void SetIntensity(float intensity) { m_AmbientIntensity = intensity; }
            inline void SetDirection(glm::vec3 direction) { m_Direction = direction; }
            inline void SetDiffuseIntensity(float intensity) { m_DiffuseIntensity = intensity; }
    };
}