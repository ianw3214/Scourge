#pragma once

#include "shade/game/entity/component/component.h"

#include <string>
#include <unordered_map>

// This probably has a better implementation, so the current one is "temporary"
class BlackboardComponent : public Shade::Component {
public:
    BlackboardComponent();

    void StoreFloat(const std::string& key, float value);
    float GetFloat(const std::string& key) const;

private:
    std::unordered_map<std::string, float> mFloats;
};