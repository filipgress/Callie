#pragma once

#include <Window/Event.h>

namespace cl{
    class Layer{
        protected:
            std::string m_DebugName;

        public:
            Layer(const std::string& name = "Layer");
            virtual ~Layer() = default;

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate(float ts) {}
            virtual void OnEvent(Event& event) {}

            const std::string& GetName() const { return m_DebugName; }
    };
}