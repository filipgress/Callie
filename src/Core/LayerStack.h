#pragma once

#include <Core/Layer.h>

namespace cl{
    class LayerStack{
        private:
            std::vector<Layer*> m_Layers;
            unsigned int m_LayerInsertIndex = 0;

        public:
            LayerStack() = default;
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);

            inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
            inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }

            inline std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
            inline std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
    };
}