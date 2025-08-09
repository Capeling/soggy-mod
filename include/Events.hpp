#pragma once 
#include <Geode/loader/Event.hpp>
#include "layers/SogLayer.hpp"

namespace soggy_mod {

class OnSogLayer : public geode::Event {
protected:
    SogLayer* m_layer;
    int m_rand;
public:
    OnSogLayer(SogLayer* layer, int rand) : m_layer(layer), m_rand(rand) {};
    SogLayer* getLayer() const { return m_layer; };
    int getRandom() const { return m_rand; };
};

}
