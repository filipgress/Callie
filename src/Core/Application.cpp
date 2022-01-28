#include <clpch.h>

#include <Core/Config.h>
#include <Core/Application.h>
#include <Graphics/Renderer.h>

unsigned int indeces[]{
    2, 0, 1,
    3, 2, 1,
    
    4, 5, 1,
    0, 4, 1,

    1, 5, 7,
    3, 1, 7,

    7, 6, 2,
    3, 7, 2,
    
    2, 6, 4,
    0, 2, 4,

    4, 6, 7,
    5, 4, 7
 };

cl::Vertex verteces[] = {
    cl::Vertex({ -1.0f, -1.0f,  1.0f }),
    cl::Vertex({  1.0f, -1.0f,  1.0f }),
    cl::Vertex({ -1.0f, -1.0f, -1.0f }),
    cl::Vertex({  1.0f, -1.0f, -1.0f }),
    cl::Vertex({ -1.0f,  1.0f,  1.0f }),
    cl::Vertex({  1.0f,  1.0f,  1.0f }),
    cl::Vertex({ -1.0f,  1.0f, -1.0f }),
    cl::Vertex({  1.0f,  1.0f, -1.0f }),
};

namespace cl{
    Application::Application(const std::string& name) {
        // Window initialization
        m_Window = std::make_unique<Window>(name);

        if(m_Window->Init() == -1){
            m_Initialized = false;
            return;
        }

        m_Window->SetEventCallback(CL_BIND_CALLBACK_FN(Application::OnEvent));

        // OpenGL initialization
        if(Renderer::Init() == -1){
            m_Initialized = false;
            return;
        }

        Renderer::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
        Renderer::SetClearColor(glm::vec4(0.13f, 0.13f, 0.13f, 1.0f));

        // Other stuff
        m_Program = std::make_unique<Shader>(c_VertexShader, c_FragmentShader);
        m_Camera = std::make_unique<cl::Camera>();
        
        m_Objects.push_back( std::make_unique<cl::Object>(verteces, sizeof(verteces), indeces, 36) );

        INFO("OpenGL Info: ");
        INFO(glGetString(GL_VERSION));
        INFO(PROJECT_NAME, "|" , PROJECT_VERSION);

        m_Initialized = true;
    }

    void Application::Run(){
        if (m_Initialized){
            glm::mat4 projection(1.0f);

            while(m_Running){
                if (!m_Minimized){
                    Renderer::Clear();

                    // Set projection matrix
                    if (m_Camera->GetProjection() == CameraMode::Perspective)
                        projection = glm::perspective(45.0f, (float)m_Window->GetWidth()/m_Window->GetHeight(), 
                            m_Camera->GetNear(), m_Camera->GetFar());
                    else
                        projection = glm::ortho(-(float)(m_Window->GetWidth()/2)*0.005f,  (float)m_Window->GetWidth()/2*0.005f, 
                                                -(float)(m_Window->GetHeight()/2)*0.005f, (float)m_Window->GetHeight()/2*0.005f,
                                                m_Camera->GetNear(), m_Camera->GetFar());

                    // Render objects
                    m_Program->Bind();
                        m_Program->SetMat4("projection", projection);
                        m_Program->SetMat4("view", m_Camera->GetView());

                        for(auto& object : m_Objects){
                            m_Program->SetMat4("model", object->GetModel());
                            m_Program->SetFloat4("setColor", object->GetColor());

                            object->Bind();
                            Renderer::Draw(object->GetVerteces(), m_Camera->GetMode());
                        }


                    m_Window->OnUpdate();
                }
            }
        }
    }

    void Application::OnEvent(Event& e){
        cl::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowResizeEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowCloseEvent>(CL_BIND_CALLBACK_FN(Application::OnWindowClose));

        dispatcher.Dispatch<cl::MouseMovedEvent>(CL_BIND_CALLBACK_FN(Application::OnMouseMoved));
        dispatcher.Dispatch<cl::MouseButtonPressedEvent>(CL_BIND_CALLBACK_FN(Application::OnMousePressed));
        dispatcher.Dispatch<cl::MouseButtonReleasedEvent>(CL_BIND_CALLBACK_FN(Application::OnMouseReleased));
    }

    bool Application::OnMouseMoved(MouseMovedEvent& e){
        return true;
    }

    bool Application::OnMousePressed(MouseButtonPressedEvent& e){
        return true;
    }
    
    bool Application::OnMouseReleased(MouseButtonReleasedEvent& e){
        return true;
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