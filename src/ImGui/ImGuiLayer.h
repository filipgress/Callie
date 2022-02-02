#pragma once

#include <Core/Layer.h>
namespace cl{
    class ImGuiLayer : public Layer{
        private:
            bool m_BlockEvents = true;
            bool m_ShowExampleWindow = true;

        public:
            ImGuiLayer();
            ~ImGuiLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnEvent(Event& e) override;
            virtual void OnUpdate(float fs) override;

            void BlockEvents(bool block) { m_BlockEvents = block; }
            void SetDarkThemeColors();
    };
}