#pragma once 
#include <Geode/loader/Event.hpp>
#include "layers/SogLayer.hpp"

namespace soggy_mod {

class OnSogLayer : public geode::Event {
public:
    OnSogLayer(SogLayer* layer, int rand) {};
};

}
