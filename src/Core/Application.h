#pragma once

#include <Core/LayerStack.h>
#include <Core/RenderLayer.h>

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
            LayerStack m_LayerStack;

            bool m_Running;
            bool m_Minimized;
            bool m_Initialized;
            float m_LastFrameTime;

            static Application* s_Instance;

        public:
            Application(const std::string& name);
            void Run();

            Window& GetWindow() { return *m_Window; }
            static Application& Get() { return *s_Instance; }

        private:
            bool OnWindowClose(WindowCloseEvent& e);
            bool OnWindowResize(WindowResizeEvent& e);
            void OnEvent(Event& e);
    };
}