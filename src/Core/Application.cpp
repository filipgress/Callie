#include <clpch.h>

#include <Graphics/Renderer.h>
#include <Core/Config.h>
#include <Core/Application.h>
#include <ImGui/ImGuiLayer.h>

namespace cl{
    Application* Application::s_Instance = nullptr;
    Application::Application(const std::string& name) 
        : m_Running(true), m_Minimized(false), m_Initialized(false) {
        s_Instance = this;

        // Window initialization
        m_Window = std::make_unique<Window>(name);

        if(m_Window->Init() == -1)
            return;

        m_Window->SetEventCallback(CL_BIND_CALLBACK_FN(Application::OnEvent));

        // OpenGL initializatioiln
        if(Renderer::Init() == -1)
            return;

        Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
        Renderer::SetClearColor(glm::vec4(0.13f, 0.13f, 0.13f, 1.0f));

        // LayerStack
        m_LayerStack.PushLayer(new ImGuiLayer());
        m_LayerStack.PushLayer(new RenderLayer());

        // Introduce yourself
        INFO("OpenGL Info: ");
        INFO(glGetString(GL_VERSION));
        INFO(PROJECT_NAME, "|" , PROJECT_VERSION);

        m_Initialized = true;
    }

    void Application::Run(){
        if (m_Initialized){
            while(m_Running){
                float time = (float)glfwGetTime();
                float deltaTime = time - m_LastFrameTime;
                m_LastFrameTime = time;
                
                if (!m_Minimized){
                    Renderer::Clear();

                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(deltaTime);

                    m_Window->OnUpdate();
                }
            }
        }
    }

    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowResizeEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowCloseEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowClose));

        for (auto layer = m_LayerStack.rbegin(); layer != m_LayerStack.rend(); layer++){
            if (e.isHandled())
                break;
            (*layer)->OnEvent(e);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e){
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e){
        if (e.GetWidth() == 0 || e.GetHeight() == 0){
            m_Minimized = true;
        }
        
        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return true;
    }
}