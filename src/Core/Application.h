#pragma once

#include <Core/Object.h>

#include <Window/Window.h>
#include <Window/ApplicationEvent.h>
#include <Window/MouseEvent.h>
#include <Window/KeyEvent.h>

#include <Graphics/Camera.h>
#include <Graphics/Shader.h>

namespace cl{
    class Application{
        private:
            Scope<Window> m_Window;
            Scope<Shader> m_Program;
            Scope<Camera> m_Camera;
            std::vector<Scope<Object>> m_Objects;

            bool m_Running;
            bool m_Minimized;
            bool m_Initialized;

            const std::string& c_VertexShader   = "../res/shaders/VertexShader.shader";
            const std::string& c_FragmentShader = "../res/shaders/FragmentShader.shader";

        public:
            Application(const std::string& name);
            void Run();

        private:
            bool OnWindowClose(WindowCloseEvent& e);
            bool OnWindowResize(WindowResizeEvent& e);

            bool OnMouseMoved(MouseMovedEvent& e);
            bool OnMousePressed(MouseButtonPressedEvent& e);
            bool OnMouseReleased(MouseButtonReleasedEvent& e);

            void OnEvent(Event& e);
    };
}