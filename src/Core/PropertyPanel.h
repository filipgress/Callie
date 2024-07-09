#pragma once

#include <imgui.h>
#include <imfilebrowser.h>

#include <Window/Event.h>
#include <Window/KeyEvent.h>

namespace cl{
    class PropertyPanel{
		using CallbackFn = std::function<bool(const std::string&)>;
        private:
            ImGui::FileBrowser m_FileDialog;
            CallbackFn m_MeshLoadCallback;

            bool m_Mode, m_Control;

        public:
            PropertyPanel();

            void OnUpdate();
            void OnEvent(Event& e);
            void SetLoadCallback(CallbackFn callback);
            inline bool GetMode() const { return m_Mode; }

        private:
            bool OnKeyPressed(KeyPressedEvent& e);
            bool OnKeyReleased(KeyReleasedEvent& e);

            void LoadObjects();
    };
}
