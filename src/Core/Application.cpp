#include <clpch.h>

#include <config.h>
#include <Core/Application.h>

#include <Graphics/Renderer.h>
#include <Graphics/UIRenderer.h>

namespace cl{
    Application* Application::s_Instance = nullptr;
    Application::Application(const std::string& name) 
        : m_Running(true), m_Minimized(false), m_Initialized(false) {
        s_Instance = this;

        // Window init 
        m_Window = std::make_unique<Window>(name);

        if(m_Window->Init() == -1)
            return;

        m_Window->SetEventCallback(CL_BIND_CALLBACK_FN(Application::OnEvent));

        // OpenGL init 
        if(Renderer::Init() == -1)
            return;

        Renderer::SetClearColor(glm::vec4(0.13f, 0.13f, 0.13f, 1.0f));

        // UI init
        UIRenderer::Init();

        m_Scene = std::make_unique<SceneView>();
        m_Panel = std::make_unique<PropertyPanel>();

        m_Panel->SetLoadCallback(CL_BIND_CALLBACK_FN(Application::OnLoadFile));

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
                    Renderer::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
                    Renderer::Clear();
                    UIRenderer::Clear();

                    m_Scene->OnUpdate(deltaTime);
                    m_Panel->OnUpdate();

                    UIRenderer::Render();
                    m_Window->OnUpdate();
                }
            }
        }
    }

    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowResizeEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowCloseEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowClose));

        if (!e.isHandled())
            m_Panel->OnEvent(e);

        if (!e.isHandled())
            m_Scene->OnEvent(e);
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

    bool Application::OnLoadFile(const std::string& filename){
        return m_Scene->LoadMesh(filename, m_Panel->GetMode());
    }
}
