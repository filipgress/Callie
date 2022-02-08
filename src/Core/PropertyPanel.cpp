#include <clpch.h>

#include <Core/PropertyPanel.h>
#include <Core/Application.h>

/**
 * TODO:
 *      - Quads loading
 *      - Directional light
 *      - Recently opened
 *      - Close
 *      - Grid
 *      - Axis Indicator
 */

namespace cl{
    PropertyPanel::PropertyPanel()
        : m_Control(false) {
        m_FileDialog.SetFileFilters({ ".obj" });
    }

    PropertyPanel::~PropertyPanel(){
        std::ofstream file("internal_saving_file.bin", 
            std::ios::out | std::ios::binary);

        if (!file.is_open()){
            CL_CORE_ASSERT("Unable to save state of program.");
        }

        file << m_OpenedFiles.str();
        file.close();
    }

    void PropertyPanel::SetLoadCallback(CallbackFn callback) { 
        m_MeshLoadCallback = callback; 
        LoadObjects();
    }

    void PropertyPanel::OnUpdate(){
		Application& app = Application::Get();
        SceneView& scene = app.GetScene();

        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                if (ImGui::MenuItem("Open", "Ctrl+o")) {
                    m_FileDialog.SetTitle("Open");
                    m_Mode = true;
                    m_FileDialog.Open();
                }
                if (ImGui::MenuItem("Import", "Ctrl-i")){
                    m_FileDialog.SetTitle("Import");
                    m_Mode = false;
                    m_FileDialog.Open();
                }
                if (ImGui::BeginMenu("Open Recent")){
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }

                ImGui::Separator();
                if (ImGui::MenuItem("Close all", "Ctrl+x")) {
                    scene.m_Objects.clear();
                    m_OpenedFiles.str("../examples/cube.obj");
                }
                if (ImGui::BeginMenu("Close")){
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Quit", "Alt+F4")) {
                    app.Close();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Wireframe", "Ctrl+w", scene.m_Camera->GetMode())) {
                    scene.m_Camera->ToggleWireframe();
                }
                if (ImGui::MenuItem("Orthographical", "Ctrl+q", (bool)scene.m_Camera->GetProjection())) {
                    scene.m_Camera->ToggleProjection();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Grid", NULL, true)) {}
                if (ImGui::MenuItem("Axis indicator", NULL, true)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("Properties");
        for (auto& object : scene.m_Objects){
            const std::string& name = object->GetName();
            if (ImGui::CollapsingHeader(name.c_str())) {
                ImGui::ColorPicker3(("Color##"+name).c_str(), &object->GetColor()[0], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                // Translate
                if (ImGui::TreeNode(("Position##"+name).c_str())){
                    ImGui::DragFloat(("X##pos"+name).c_str(), &object->GetPos()[0], 0.03f);
                    ImGui::DragFloat(("Y##pos"+name).c_str(), &object->GetPos()[1], 0.03f);
                    ImGui::DragFloat(("Z##pos"+name).c_str(), &object->GetPos()[2], 0.03f);
                    ImGui::TreePop();
                }

                // // Rotate 
                if (ImGui::TreeNode(("Rotate##"+name).c_str())){
                    ImGui::DragFloat(("X##rot"+name).c_str(), &object->GetRot()[0], 0.1f);
                    ImGui::DragFloat(("Y##rot"+name).c_str(), &object->GetRot()[1], 0.1f);
                    ImGui::DragFloat(("Z##rot"+name).c_str(), &object->GetRot()[2], 0.1f);
                    ImGui::TreePop();
                }

                // // Scale 
                if (ImGui::TreeNode(("Scale##"+name).c_str())){
                    ImGui::DragFloat(("X##scale"+name).c_str(), &object->GetScale()[0], 0.03f);
                    ImGui::DragFloat(("Y##scale"+name).c_str(), &object->GetScale()[1], 0.03f);
                    ImGui::DragFloat(("Z##scale"+name).c_str(), &object->GetScale()[2], 0.03f);
                    ImGui::TreePop();
                }
            }
        }
        if (ImGui::CollapsingHeader("Light")) {}
        ImGui::End();

        m_FileDialog.Display();
        if (m_FileDialog.HasSelected()){
            auto file_path = m_FileDialog.GetSelected().string();
            
            if (m_MeshLoadCallback(file_path)){
                if(m_Mode){
                    m_OpenedFiles.str("");
                }
                m_OpenedFiles << file_path << "\n";
            }

            m_FileDialog.ClearSelected();
        }

        if (m_FileDialog.IsOpened()){
            scene.m_KeyInput = false;
        } else{
            scene.m_KeyInput = true;
        }
    }

    void PropertyPanel::OnEvent(Event& e){
        cl::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<cl::KeyPressedEvent>(CL_BIND_CALLBACK_FN(PropertyPanel::OnKeyPressed));
        dispatcher.Dispatch<cl::KeyReleasedEvent>(CL_BIND_CALLBACK_FN(PropertyPanel::OnKeyReleased));
    }

    bool PropertyPanel::OnKeyPressed(KeyPressedEvent& e){
		Application& app = Application::Get();
        SceneView& scene = app.GetScene();

        KeyCode key = e.GetKeyCode();
        if (key == Key::LeftControl)
            m_Control = true;

        if (m_Control){
            switch(key){
                case Key::O:
                    if (!m_FileDialog.IsOpened()){
                        m_FileDialog.SetTitle("Open");
                        m_Mode = true;
                        m_FileDialog.Open();
                    }
                    break;
                case Key::I:
                    if (!m_FileDialog.IsOpened()){
                        m_FileDialog.SetTitle("Import");
                        m_Mode = false;
                        m_FileDialog.Open();
                    }
                    break;
                case Key::X:
                    scene.m_Objects.clear();
                    m_OpenedFiles.str("../examples/cube.obj");
                    break; 
            }
        }
        if (key == Key::O && m_Control){
            if (!m_FileDialog.IsOpened())
                m_FileDialog.Open();
            return true;
        }
        
        return false;
    }

    bool PropertyPanel::OnKeyReleased(KeyReleasedEvent& e){
        if (e.GetKeyCode() == Key::LeftControl)
            m_Control = false;
        
        return false;
    }

    void PropertyPanel::LoadObjects(){
        std::ifstream stream("internal_saving_file.bin", std::ios::in | std::ios::binary);

        if(!stream.is_open()){
            CL_CORE_ASSERT("Unable to load previous session! Internal file: 'internal_saving_file.bin' is not accessible!");
        }

        std::string line;
        while(getline(stream, line)){
            if (m_MeshLoadCallback(line)){
                m_OpenedFiles << line << "\n";
            }
        }

        stream.close();
    }
}