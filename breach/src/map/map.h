#pragma once

#include <memory>
#include <vector>
#include <string>

#include "shade/resource/handle.h"
#include "shade/resource/resource.h"

#include "map/layout.h"

// ======================================
struct BackgroundElement {
    std::string mName;
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