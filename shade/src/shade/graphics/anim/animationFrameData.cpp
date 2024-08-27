#include "animationFrameData.h"

#include "shade/file/fileSystem.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

// ======================================
Shade::Resource* Shade::AnimationFrameData::Load(const std::string& path)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();
    std::unique_ptr<Shade::JsonFile> file = fileSystem->LoadJsonFile(path);

    if (file == nullptr)
    {
        logger->LogError(std::string("Could not load Animation frame data, file was not loaded: ") + path);
        return nullptr;
    }

    const nlohmann::json& contents = file->GetContents();
    if (!contents.contains("meta"))
    {
        logger->LogError(std::string("Animation frame data missing meta data, file was not loaded: ") + path);
        return nullptr;
    }

    const nlohmann::json& meta = contents["meta"];
    if (!meta.contains("frameTags"))
    {
        logger->LogError(std::string("Animation frame data missing frame tag data, file was not loaded: ") + path);
        return nullptr;
    }

    const uint16_t width = static_cast<uint16_t>(meta["size"]["w"]);
    const uint16_t height = static_cast<uint16_t>(meta["size"]["h"]);
    const nlohmann::json& frameTags = meta["frameTags"];
    if (!frameTags.is_array())
    {
        logger->LogError(std::string("Animation frame tag data is not in array format, file was not loaded: ") + path);
        return nullptr;
    }

    std::vector<AnimationFrameInfo> animationFrames;
    for (const nlohmann::json& frameTag : frameTags)
    {
        const std::string& name = frameTag["name"];
        const uint16_t from = static_cast<uint16_t>(frameTag["from"]);
        const uint16_t to = static_cast<uint16_t>(frameTag["to"]);
        animationFrames.emplace_back(AnimationFrameInfo{ name, from, to });
    }

    const nlohmann::json& frames = contents["frames"];
    const nlohmann::json& firstFrameInfo = frames.front()["frame"];
    const uint16_t frameWidth = firstFrameInfo["w"];
    const uint16_t frameHeight = firstFrameInfo["h"];

    const uint16_t rows = height / frameHeight;
    const uint16_t columns = width / frameWidth;
    return new AnimationFrameData(frameWidth, frameHeight, rows, columns, std::move(animationFrames));
}

// ======================================
Shade::AnimationFrameData::AnimationFrameData(uint16_t w, uint16_t h, uint16_t rows, uint16_t columns, std::vector<AnimationFrameInfo>&& animationFrames)
    : mFrameWidth(w)
    , mFrameHeight(h)
    , mRows(rows)
    , mColumns(columns)
    , mAnimationFrames(animationFrames)
{
    
}

// ======================================
const std::vector<Shade::AnimationFrameInfo>& Shade::AnimationFrameData::GetAnimationFrames() const
{
    return mAnimationFrames;
}

// ======================================
const uint16_t Shade::AnimationFrameData::GetFrameWidth() const
{
    return mFrameWidth;
}

// ======================================
const uint16_t Shade::AnimationFrameData::GetFrameHeight() const
{
    return mFrameHeight;
}

// ======================================
const uint16_t Shade::AnimationFrameData::GetRows() const
{
    return mRows;
}

// ======================================
const uint16_t Shade::AnimationFrameData::GetColumns() const
{
    return mColumns;
}
