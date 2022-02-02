#include <clpch.h>

#include <Core/LayerStack.h>

namespace cl{
    Layer::Layer(const std::string& name) : m_DebugName(name) {}

    LayerStack::~LayerStack(){
        for (Layer* layer : m_Layers)
            delete layer;
    }

    void LayerStack::PushLayer(Layer* layer){
        layer->OnAttach();
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay){
        overlay->OnAttach();
        m_Layers.emplace_back(overlay);
    }
}