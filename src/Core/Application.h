#pragma once

#include <Core/PropertyPanel.h>
#include <Core/SceneView.h>

#include <GL/glew.h>
#include <Window/Window.h>
#include <Window/ApplicationEvent.h>

namespace cl{
    struct ApplicationCommandLineArgs{
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const{
            CL_ASSERT(index < Count);
            return Args[index];
        }
    };

    class Application{
        private:
            ApplicationCommandLineArgs m_CommandLineArgs;
            Scope<Window> m_Window;

            Scope<PropertyPanel> m_Panel;
            Scope<SceneView> m_Scene;

            bool m_Running;
            bool m_Minimized;

            bool m_Initialized;
            float m_LastFrameTime;

            static Application* s_Instance;

        public:
            Application(const std::string& name);
            void Run();
            inline void Close() { m_Running = false; }

            Window& GetWindow() { return *m_Window; }
            SceneView& GetScene() { return *m_Scene; }
            static Application& Get() { return *s_Instance; }


        private:
            bool OnWindowClose(WindowCloseEvent& e);
            bool OnWindowResize(WindowResizeEvent& e);
            bool OnLoadFile(const std::string& filename);
            void OnEvent(Event& e);
    };
}