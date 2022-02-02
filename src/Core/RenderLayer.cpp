#include <clpch.h>

#include <Core/RenderLayer.h>
#include <Graphics/Renderer.h>
#include <Core/Application.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

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
    RenderLayer::RenderLayer() : Layer("RenderLayer") {
        m_Frame = std::make_unique<FrameBuffer>();
        m_Frame->CreateBuffers(800, 500);

        m_Program = std::make_unique<Shader>(c_VertexShader, c_FragmentShader);
        m_Camera = std::make_unique<cl::Camera>();

        m_Objects.push_back( std::make_unique<cl::Object>(verteces, sizeof(verteces), indeces, 36) );
    }

    void RenderLayer::OnUpdate(float ts) {
		Application& app = Application::Get();
		Window* window = &app.GetWindow();

        // Set projection matrix
        glm::mat4 projection(1.0f);
        if (m_Camera->GetProjection() == CameraMode::Perspective)
            projection = glm::perspective(45.0f, (float)window->GetWidth()/window->GetHeight(), 
                m_Camera->GetNear(), m_Camera->GetFar());
        else
            projection = glm::ortho(-(float)(window->GetWidth()/2)*0.005f,  (float)window->GetWidth()/2*0.005f, 
                                    -(float)(window->GetHeight()/2)*0.005f, (float)window->GetHeight()/2*0.005f,
                                    m_Camera->GetNear(), m_Camera->GetFar());

        // Render objects
        m_Frame->Bind();
        m_Program->Bind();
            m_Program->SetMat4("projection", projection);
            m_Program->SetMat4("view", m_Camera->GetView());

            for(auto& object : m_Objects){
                m_Program->SetMat4("model", object->GetModel());
                m_Program->SetFloat4("setColor", object->GetColor());

                object->Bind();
                Renderer::Draw(object->GetVerteces(), m_Camera->GetMode());
            }
        m_Program->Unbind();
        m_Frame->Unbind();

        ImGui::Begin("Scene");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        glm::vec2 mSize = { viewportPanelSize.x, viewportPanelSize.y };

        // add rendered texture to ImGUI scene window
        uint64_t textureID = m_Frame->GetTexture();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        m_Program->Bind();

        ImGui::End();
    }

    void RenderLayer::OnEvent(Event& e) {
        cl::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<cl::MouseMovedEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnMouseMoved));
        dispatcher.Dispatch<cl::MouseScrolledEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnScrolled));
        dispatcher.Dispatch<cl::MouseButtonPressedEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnMousePressed));
        dispatcher.Dispatch<cl::MouseButtonReleasedEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnMouseReleased));
        
        dispatcher.Dispatch<cl::KeyPressedEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnKeyPressed));
        dispatcher.Dispatch<cl::KeyReleasedEvent>(CL_BIND_CALLBACK_FN(RenderLayer::OnKeyReleased));
    }

    /* Mouse events */
    bool RenderLayer::OnMouseMoved(MouseMovedEvent& e){
		Application& app = Application::Get();
		Window* window = &app.GetWindow();

        m_Moved.x = e.GetX();
        m_Moved.y = e.GetY();
        
        if (m_MouseStatus[cl::Mouse::ButtonLeft]){
            m_Camera->RotateAroundDefault((m_Moved.x - m_Pressed.x)/window->GetWidth(), 
                (m_Moved.y - m_Pressed.y)/window->GetHeight());

            m_Pressed = m_Moved;
            return true;
        }
        if (m_MouseStatus[cl::Mouse::ButtonRight]){
            m_Camera->Translate((m_Moved.x - m_Pressed.x)/window->GetWidth(),
                (m_Moved.y - m_Pressed.y)/window->GetHeight());

            m_Pressed = m_Moved;
            return true;
        }

        return false;
    }

    bool RenderLayer::OnMousePressed(MouseButtonPressedEvent& e){
        m_MouseStatus[e.GetMouseButton()] = true;
        m_Pressed = m_Moved;

        return true;
    }
    
    bool RenderLayer::OnMouseReleased(MouseButtonReleasedEvent& e){
        m_MouseStatus[e.GetMouseButton()] = false;

        return true;
    }

    bool RenderLayer::OnScrolled(MouseScrolledEvent& e){
        m_Camera->Zoom(e.GetYOffset()*0.01f);
        return true;
    }
    
    /* KEY events */
    bool RenderLayer::OnKeyPressed(KeyPressedEvent& e){
        m_KeyStatus[e.GetKeyCode()] = true;

        switch (e.GetKeyCode()){
            case cl::Key::W:
                m_Camera->ToggleWireframe();
                break;
            case cl::Key::A:
                m_Camera->ToggleProjection();
                break;
        }

        return true;
    }

    bool RenderLayer::OnKeyReleased(KeyReleasedEvent& e){
        m_KeyStatus[e.GetKeyCode()] = false;
        return true;
    }
}