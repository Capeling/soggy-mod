#pragma once 
#include <Geode/loader/Event.hpp>
#include "layers/SogLayer.hpp"

namespace soggy_mod {
    class OnSogLayer : public geode::Event {
    protected:
        SogLayer* m_layer;
    public:
        OnSogLayer(SogLayer* layer, int rand) : m_layer(layer) {};
        SogLayer* getLayer() const { return m_layer; };
    };
}
