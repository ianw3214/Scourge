#pragma once

#include "shade/common/collision.h"
#include "shade/common/vec.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/resource/resource.h"

#include <memory>
#include <string>
#include <vector>

// ======================================
struct BackgroundElement {
    std::string mTexturePath;
    float mParallax = 0.f;
};

// ======================================
class MapLayout : public Shade::Resource {
public:
    static Shade::Resource* Load(const std::string& path);

public:
    MapLayout(std::vector<Shade::Box>&& playZones, std::vector<BackgroundElement>&& backgrounds);

    // TODO: Make this somehow not have to pass in the entire game world to create the entities
    std::vector<std::unique_ptr<Shade::Entity>> CreateGameEntities(Shade::GameWorldModule& gameWorld);

    const std::vector<Shade::Box>& GetPlayZones() const;
private:
    // Gameplay related layout
    std::vector<Shade::Box> mPlayZones;
    Shade::Vec2 mPlayerStart;

    // Visual layout
    // TODO: Consider seperating out the map visuals into something else
    //  - These are only needed when the map is loaded, whereas the layout is something that is re-used over and over
    std::vector<BackgroundElement> mBackgrounds;
};