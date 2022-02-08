#include <clpch.h>

#include <Core/Application.h>
#include <Graphics/Renderer.h>
#include <Core/SceneView.h>

namespace cl{
    SceneView::SceneView() 
        : m_KeyStatus{0}, m_MouseStatus{0} {
        m_Frame = std::make_unique<FrameBuffer>();
        m_Frame->CreateBuffers(800, 500);

        m_Program = std::make_unique<Shader>(c_VertexShader, c_FragmentShader);
        m_Camera = std::make_unique<cl::Camera>();

        // LoadMesh("../examples/cube.obj");
    }

    void SceneView::OnUpdate(float ts) {
		Application& app = Application::Get();
		Window* window = &app.GetWindow();

        ImGui::Begin("Scene");
        if (ImGui::IsWindowHovered()){
            m_Blocked = false;
        } else{
            m_Blocked = true;
        }

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        glm::vec2 size = { viewportPanelSize.x, viewportPanelSize.y };

        // Set projection matrix
        float aspectRatio = size.x / size.y;
        float distance = m_Camera->GetDistance()*0.52f;

        glm::mat4 projection(1.0f);
        if (m_Camera->GetProjection() == CameraMode::Perspective)
            projection = glm::perspective(45.0f, aspectRatio,
                m_Camera->GetNear(), m_Camera->GetFar());
        else
            projection = glm::ortho(-distance*aspectRatio, distance*aspectRatio, 
                -distance, distance, 
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

        // add rendered texture to ImGUI scene window
        uint64_t textureID = m_Frame->GetTexture();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        m_Program->Bind();

        ImGui::End();
    }

    // very basic implementation...
    // not all files will be able to open correctly
    // but for now it works just fine i guess
    bool SceneView::LoadMesh(const std::string& filepath, bool clear){
        std::string name = filepath.substr(filepath.find_last_of("/\\") + 1);

        std::ifstream stream(filepath, std::ios::in);
        if(!stream.is_open()){
            CL_CORE_ASSERT("Failed to open file:", filepath);

            return false;
        }

        std::vector<Vertex> verteces;
        std::vector<unsigned int> indeces;

        std::string line;
        while(getline(stream, line)){
            std::stringstream ss(line);
            std::string id;
            ss >> id;

            if (id == "v"){
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;

                verteces.push_back(Vertex(v));
            } 
            else if (id == "f"){
                // TODO: implement quads, normals, UVs
                std::string v1, v2,  v3;
                ss >> v1 >> v2 >> v3;

                uint32_t vert_idx[3];
                vert_idx[0] = tokenize(v1, '/').at(0);
                vert_idx[1] = tokenize(v2, '/').at(0);
                vert_idx[2] = tokenize(v3, '/').at(0);

                indeces.push_back(vert_idx[0] - 1);
                indeces.push_back(vert_idx[1] - 1);
                indeces.push_back(vert_idx[2] - 1);
            }
        }

        if (clear)
            m_Objects.clear();
        m_Objects.push_back(std::make_unique<cl::Object>(name, verteces, indeces));

        stream.close();
        return true;
    }

    void SceneView::OnEvent(Event& e) {
        cl::EventDispatcher dispatcher(e);

        if(!m_Blocked){
            dispatcher.Dispatch<cl::MouseScrolledEvent>(CL_BIND_CALLBACK_FN(SceneView::OnScrolled));
            dispatcher.Dispatch<cl::MouseButtonPressedEvent>(CL_BIND_CALLBACK_FN(SceneView::OnMousePressed));
        }
        if(m_KeyInput){
            dispatcher.Dispatch<cl::KeyPressedEvent>(CL_BIND_CALLBACK_FN(SceneView::OnKeyPressed));
            dispatcher.Dispatch<cl::KeyReleasedEvent>(CL_BIND_CALLBACK_FN(SceneView::OnKeyReleased));
        }

        dispatcher.Dispatch<cl::MouseMovedEvent>(CL_BIND_CALLBACK_FN(SceneView::OnMouseMoved));
        dispatcher.Dispatch<cl::MouseButtonReleasedEvent>(CL_BIND_CALLBACK_FN(SceneView::OnMouseReleased));
    }

    /* Mouse events */
    bool SceneView::OnMouseMoved(MouseMovedEvent& e){
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

    bool SceneView::OnMousePressed(MouseButtonPressedEvent& e){
        m_MouseStatus[e.GetMouseButton()] = true;
        m_Pressed = m_Moved;

        return true;
    }
    
    bool SceneView::OnMouseReleased(MouseButtonReleasedEvent& e){
        m_MouseStatus[e.GetMouseButton()] = false;

        return true;
    }

    bool SceneView::OnScrolled(MouseScrolledEvent& e){
        m_Camera->Zoom(e.GetYOffset()*0.01f);
        return true;
    }
    
    /* KEY events */
    bool SceneView::OnKeyPressed(KeyPressedEvent& e){
        KeyCode key = e.GetKeyCode();
        m_KeyStatus[key] = true;

        if (m_KeyStatus[cl::Key::LeftControl]){
            switch (key){
                case cl::Key::W:
                    m_Camera->ToggleWireframe();
                    break;
                case cl::Key::Q:
                    m_Camera->ToggleProjection();
                    break;
            }
        }
        if (m_KeyStatus[cl::Key::LeftAlt] &&
            m_KeyStatus[cl::Key::F4]){
            Application& app = Application::Get();
            app.Close();
        }

        return true;
    }

    bool SceneView::OnKeyReleased(KeyReleasedEvent& e){
        m_KeyStatus[e.GetKeyCode()] = false;
        return true;
    }

    std::vector<uint32_t> SceneView::tokenize(const std::string& line, const char token){
        std::vector<uint32_t> result;
        
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, token))
        if (!item.empty())
            result.push_back(std::stoi(item));

        return result;
    }
}