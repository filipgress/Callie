#include <clpch.h>

#include <Core/PropertyPanel.h>
#include <Core/Application.h>

namespace cl{
    PropertyPanel::PropertyPanel(){
        m_CurrentFile = "< ... >";

        m_FileDialog.SetTitle("Open");
        m_FileDialog.SetFileFilters({ ".fgx", ".obj" });
    }

    void PropertyPanel::OnUpdate(){
		Application& app = Application::Get();
        SceneView& scene = app.GetScene();

        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                if (ImGui::MenuItem("Open", "Ctrl+o")) {
                    m_FileDialog.Open();
                }
                if (ImGui::BeginMenu("Open Recent")){
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }

                ImGui::Separator();
                if (ImGui::MenuItem("Close all", "Ctrl+x")) {}
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
            m_CurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

            m_MeshLoadCallback(file_path);

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
        KeyCode key = e.GetKeyCode();
        if (key == Key::LeftControl)
            m_Control = true;

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
}