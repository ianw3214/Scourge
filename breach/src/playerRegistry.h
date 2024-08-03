#pragma once

#include "shade/game/entity/entity.h"

// ======================================
// TODO: This is a pretty hacky system, figure out a better way to track this in the future
class PlayerRegistry {
public:
    static void CachePlayer(Shade::Entity* player) { sCachedPlayer = player; }
    static Shade::Entity* GetCachedPlayer() { return sCachedPlayer; }
private:
    static inline Shade::Entity* sCachedPlayer = nullptr;
};