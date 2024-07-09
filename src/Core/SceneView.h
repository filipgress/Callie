#pragma once

#include <Core/Object.h>
#include <Graphics/Camera.h>
#include <Graphics/Shader.h>
#include <Graphics/Light.h>
#include <Graphics/FrameBuffer.h>

#include <Window/ApplicationEvent.h>
#include <Window/KeyEvent.h>
#include <Window/MouseEvent.h>

namespace cl{
    class SceneView{
        friend class PropertyPanel;
        friend bool LoadMesh(const std::string& filepath);
        private:
            Light m_Light;
            Scope<Shader> m_Program;
            Scope<Camera> m_Camera;
            Scope<FrameBuffer> m_Frame;
            std::vector<Scope<Object>> m_Objects;
        
            bool m_KeyStatus[317];
            bool m_MouseStatus[8];
            bool m_Blocked = false;
            bool m_KeyInput = true;

            glm::vec2 m_Moved, m_Pressed;

            const std::string& c_VertexShader   = "../res/shaders/VertexShader.shader";
            const std::string& c_FragmentShader = "../res/shaders/FragmentShader.shader";
            /* Axis indicator*/
            cl::Line xAxis, yAxis, zAxis;
            cl::Line Grid[15];

            bool isGrid = true, isAxis = true;

        public:
            SceneView();
            ~SceneView();

            bool LoadMesh(const std::string& filepath, bool clear = false);
            void OnUpdate(float ts);
            void OnEvent(Event& e);

        private:
            bool OnMouseMoved(MouseMovedEvent& e);
            bool OnScrolled(MouseScrolledEvent& e);
            bool OnMousePressed(MouseButtonPressedEvent& e);
            bool OnMouseReleased(MouseButtonReleasedEvent& e);

            bool OnKeyPressed(KeyPressedEvent& e);
            bool OnKeyReleased(KeyReleasedEvent& e);

            std::vector<uint32_t> tokenize(const std::string& line, const char token);
    };
}