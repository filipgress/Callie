#pragma once

#include <imgui.h>
#include <Plugins/ImFileBrowser.h>
#include <Window/Event.h>
#include <Window/KeyEvent.h>

namespace cl{
    class PropertyPanel{
		using CallbackFn = std::function<void(const std::string&)>;
        private:
            ImGui::FileBrowser m_FileDialog;
            std::string m_CurrentFile;
            CallbackFn m_MeshLoadCallback;
            bool m_Control = false;

        public:
            PropertyPanel();

            void OnUpdate();
            void OnEvent(Event& e);
            void SetLoadCallback(CallbackFn callback) { m_MeshLoadCallback = callback; }

        private:
            bool OnKeyPressed(KeyPressedEvent& e);
            bool OnKeyReleased(KeyReleasedEvent& e);
    };
}