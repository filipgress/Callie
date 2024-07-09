#include <clpch.h>

#include <Core/Application.h>
#include <Graphics/Renderer.h>
#include <Core/SceneView.h>


namespace cl{

    SceneView::SceneView() 
        : m_KeyStatus{0}, m_MouseStatus{0},
          xAxis({ -1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f, 0.0f }, {1.0f, 0.2f, 0.2f, 1.0f}),
          yAxis({  0.0f, -1.0f,  0.0f }, { 0.0f,  1.0f, 0.0f }, {0.2f, 1.0f, 0.2f, 1.0f}),
          zAxis({  0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, 1.0f }, {0.4f, 0.4f, 1.0f, 1.0f}),
          Grid{
            cl::Line({ -3.0f,  0.0f, -3.0f }, { 3.0f, 0.0f, -3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -3.0f,  0.0f, -2.0f }, { 3.0f, 0.0f, -2.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -3.0f,  0.0f, -1.0f }, { 3.0f, 0.0f, -1.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -5.0f,  0.0f,  0.0f }, { 5.0f, 0.0f,  0.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -3.0f,  0.0f,  1.0f }, { 3.0f, 0.0f,  1.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -3.0f,  0.0f,  2.0f }, { 3.0f, 0.0f,  2.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -3.0f,  0.0f,  3.0f }, { 3.0f, 0.0f,  3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),

            cl::Line({ -3.0f,  0.0f, -3.0f }, {-3.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -2.0f,  0.0f, -3.0f }, {-2.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({ -1.0f,  0.0f, -3.0f }, {-1.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({  0.0f,  0.0f, -5.0f }, { 0.0f,  0.0f, 5.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({  1.0f,  0.0f, -3.0f }, { 1.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({  2.0f,  0.0f, -3.0f }, { 2.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),
            cl::Line({  3.0f,  0.0f, -3.0f }, { 3.0f,  0.0f, 3.0f }, {0.3f, 0.3f, 0.3f, 1.0f}),

            cl::Line({  0.0f, -5.0f, 0.0f }, { 0.0f,  5.0f, 0.0f }, {0.5f, 0.5f, 0.5f, 1.0f})
          } {
        m_Frame = std::make_unique<FrameBuffer>();
        m_Frame->CreateBuffers(1200, 1200);

        m_Program = std::make_unique<Shader>(c_VertexShader, c_FragmentShader);
        m_Camera = std::make_unique<cl::Camera>();
    }

    SceneView::~SceneView(){
        std::ofstream file("internal_saving_file.bin", 
            std::ios::out | std::ios::binary);

        if (!file.is_open()){
            CL_CORE_ASSERT("Unable to save state of program.");
        }

        for (auto& object : m_Objects){
            file << object->GetFilepath() << "\n";
        }
        file.close();
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

        m_Frame->Bind();
        m_Program->Bind();
            m_Program->SetMat4("projection", projection);
            m_Program->SetMat4("view", m_Camera->GetView());

            /* Grid */
            if (isGrid){
                m_Program->SetMat4("model", glm::mat4(1.0f));
                m_Program->SetFloat4("col", Grid[0].GetColor());
                for(int i=0; i<15; i++)
                    Grid[i].Draw();
            }

            /* Render objects */
            m_Program->SetFloat4("directionalLight.colour", m_Light.GetColor());
            m_Program->SetFloat("directionalLight.ambientIntensity", m_Light.GetIntensity());
            m_Program->SetFloat3("directionalLight.direction", m_Light.GetDirection());
            m_Program->SetFloat("directionalLight.diffuseIntensity", m_Light.GetDiffuseIntensity());

            for(auto& object : m_Objects){
                m_Program->SetMat4("model", object->GetModel());
                m_Program->SetFloat4("col", object->GetColor());

                object->Bind();
                Renderer::Draw(object->GetVerteces(), m_Camera->GetMode());
            }

            /* Axis indicator*/
            m_Program->SetFloat4("directionalLight.colour", glm::vec4(1.0f));
            m_Program->SetFloat("directionalLight.ambientIntensity", 1.0f);

            if (isAxis){
                Renderer::SetViewport(25, 25, 100, 100);
                
                projection = glm::perspective(45.0f, (GLfloat)aspectRatio, 0.1f, 100.0f);
                m_Program->SetMat4("projection", projection);
                m_Program->SetMat4("view", glm::mat4(1.0f));
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.1f)) * m_Camera->GetRotate();
                m_Program->SetMat4("model", model);

                m_Program->SetFloat4("col", xAxis.GetColor());
                xAxis.Draw();
                m_Program->SetFloat4("col", yAxis.GetColor());
                yAxis.Draw();
                m_Program->SetFloat4("col", zAxis.GetColor());
                zAxis.Draw();
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
    // TODO: quads and other instances, faces, lines, textures
    bool SceneView::LoadMesh(const std::string& filepath, bool clear){
        std::ifstream stream(filepath, std::ios::in);
        if(!stream.is_open()){
            CL_CORE_ASSERT("Failed to open file:", filepath);

            return false;
        }

        std::vector<Vertex> vertices;

        std::vector<glm::vec3> vertex_positions;
        std::vector<glm::vec3> vertex_normals;

        std::map<unsigned int, unsigned int> indecies;
        std::vector<unsigned int> vertex_indecies;
        // std::vector<unsigned int> vertex_normal_indecies;

        std::string line;
        while(getline(stream, line)){
            std::stringstream ss(line);
            std::string id;
            ss >> id;

            if (id == "v"){ // vertex
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;

                vertex_positions.push_back(v);
            } 
            else if (id == "vn"){ // normal
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;

                vertex_normals.push_back(v);
            }
            else if (id == "f"){
                std::string v;
                while(ss >> v){
                    auto token = tokenize(v, '/');

                    vertex_indecies.push_back(token.at(0)-1);
                    indecies[token.at(0)-1] = token.at(2)-1;
                    // vertex_normal_indecies.push_back(token.at(2)-1);
                }
            }
        }

        if (clear)
            m_Objects.clear();

        for (int i=0; i < vertex_positions.size(); i++){
            vertices.push_back(Vertex(vertex_positions[i], 
                vertex_normals[indecies[i]]));
        } 

        m_Objects.push_back(std::make_unique<cl::Object>(filepath, vertices, vertex_indecies));

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
        while (std::getline(ss, item, token)){
            if (!item.empty())
                result.push_back(std::stoi(item));
            else
                result.push_back(0);
        }

        return result;
    }
}