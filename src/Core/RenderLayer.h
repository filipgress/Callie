#pragma once

#include <Core/Layer.h>
#include <Core/Object.h>
#include <Graphics/Camera.h>
#include <Graphics/Shader.h>
#include <Graphics/FrameBuffer.h>

#include <Window/ApplicationEvent.h>
#include <Window/KeyEvent.h>
#include <Window/MouseEvent.h>

namespace cl{
    class RenderLayer : public Layer{
        private:
            Scope<Shader> m_Program;
            Scope<Camera> m_Camera;
            Scope<FrameBuffer> m_Frame;
            std::vector<Scope<Object>> m_Objects;

            bool m_KeyStatus[317];
            bool m_MouseStatus[8];

            glm::vec2 m_Moved, m_Pressed;

            const std::string& c_VertexShader   = "../res/shaders/VertexShader.shader";
            const std::string& c_FragmentShader = "../res/shaders/FragmentShader.shader";

            void OnUpdate(float ts);
            void OnEvent(Event& e);

            bool OnMouseMoved(MouseMovedEvent& e);
            bool OnScrolled(MouseScrolledEvent& e);
            bool OnMousePressed(MouseButtonPressedEvent& e);
            bool OnMouseReleased(MouseButtonReleasedEvent& e);

            bool OnKeyPressed(KeyPressedEvent& e);
            bool OnKeyReleased(KeyReleasedEvent& e);

        public:
            RenderLayer();
            ~RenderLayer() = default;
    };
}