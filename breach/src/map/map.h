#pragma once

#include <memory>
#include <vector>
#include <string>

#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/instance/service/service.h"
#include "shade/resource/handle.h"
#include "shade/resource/resource.h"

#include "map/layout.h"

// ======================================
struct BackgroundElement {
    std::string mTexturePath;
    float mParallax = 0.f;
};

// ======================================
class MapData : public Shade::Resource {
public:
    static Shade::Resource* Load(const std::string& path);

public:
    MapData(const std::string& name, std::vector<BackgroundElement>&& backgrounds, const MapLayout&& layout);

    const std::string& GetName() const { return mName; }
    const std::vector<BackgroundElement>& GetBackgrounds() const { return mBackgrounds; }
    const MapLayout& GetLayout() const { return mLayout; }
private:
    std::string mName;
    std::vector<BackgroundElement> mBackgrounds;
    MapLayout mLayout;
};

// ======================================
// TODO: Maybe separate into it's own file?
class MapService : public Shade::Service {
public:
    MapService();
    ~MapService();

    // TODO: Make this somehow not have to pass in the entire game world to create the entities
    //  - Perhaps an entity factory can be passed in for creating entities
    //  - Alternatively, register entity factory as a service and locate the service to create new entities
    std::vector<std::unique_ptr<Shade::Entity>> LoadMap(const std::string& path, Shade::GameWorldModule& gameWorld);

    const MapLayout& GetLayout() const;

protected:
    void SetMapLayout(MapLayout layout);

private:
    MapLayout mCurrentLayout;
};
