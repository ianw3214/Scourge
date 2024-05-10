#include "keyValueFile.h"

#include <vector>
#include <limits>

#include "shade/common/stringUtil.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

namespace {

    // TODO: Can this be combined with the trim to avoid duplicate work
    uint8_t CountDepth(const std::string& input)
    {
        size_t index = 0;
        while (index < input.size())
        {
            if (std::isspace(input[index])) {
                index++;
            }
            else {
                break;
            }
        }
        // TODO: Should assert that this is divisible by 2 in case there's error in the data
        return static_cast<uint8_t>(index / 2);
    }

}

// TODO: Asserts on key value handle access
//  - Make sure index is within size
//  - Make sure "get" functions have the correct type

// ======================================
Shade::KeyValueHandle::KeyValueHandle(const std::vector<KeyValuePair>& bufferRef)
    : mBufferRef(bufferRef)
{

}

// ======================================
Shade::KeyValueHandle::KeyValueHandle(const std::vector<KeyValuePair>& bufferRef, size_t index)
    : mBufferRef(bufferRef)
    , mIndex(index)
{

}

// ======================================
Shade::KeyValueHandle Shade::KeyValueHandle::Invalid(const std::vector<KeyValuePair>& bufferRef) const
{
    return KeyValueHandle(bufferRef, std::numeric_limits<size_t>::max());
}

// ======================================
void Shade::KeyValueHandle::Invalidate()
{
    mIndex = std::numeric_limits<size_t>::max();
}

// ======================================
bool Shade::KeyValueHandle::IsValid() const
{
    return mBufferRef.size() > mIndex;
}

// ======================================
bool Shade::KeyValueHandle::ToNext()
{
    // Search for the next item at the same depth
    uint8_t currDepth = mBufferRef[mIndex].mDepth;
    do {
        mIndex++;
        if (mIndex >= mBufferRef.size())
        {
            break;
        }
        if (mBufferRef[mIndex].mDepth < currDepth)
        {
            Invalidate();
            break;
        }
    } while (mBufferRef[mIndex].mDepth != currDepth);
    return IsValid();
}

// ======================================
bool Shade::KeyValueHandle::IsInt() const
{
    return mBufferRef[mIndex].mValue.mType == ValueType::INT;
}

// ======================================
bool Shade::KeyValueHandle::IsFloat() const
{
    return mBufferRef[mIndex].mValue.mType == ValueType::FLOAT;
}

// ======================================
bool Shade::KeyValueHandle::IsString() const
{
    return mBufferRef[mIndex].mValue.mType == ValueType::STRING;
}

// ======================================
bool Shade::KeyValueHandle::IsList() const
{
    return mBufferRef[mIndex].mValue.mType == ValueType::LIST;
}

// ======================================
const std::string& Shade::KeyValueHandle::GetKey() const
{
    return mBufferRef[mIndex].mKey;
}

// ======================================
int Shade::KeyValueHandle::GetInt() const
{
    return mBufferRef[mIndex].mValue.mInt;
}

// ======================================
float Shade::KeyValueHandle::GetFloat() const
{
    return mBufferRef[mIndex].mValue.mFloat;
}

// ======================================
const std::string& Shade::KeyValueHandle::GetString() const
{
    return mBufferRef[mIndex].mValue.mString;
}

// ======================================
Shade::KeyValueHandle Shade::KeyValueHandle::GetListHead() const
{
    // TODO: More error checking on the format
    return KeyValueHandle(mBufferRef, mIndex + 1);
}

// ======================================
Shade::KeyValueFile::KeyValueFile(std::vector<KeyValuePair>&& buffer)
    : mBuffer(std::move(buffer))
{

}

// ======================================
std::unique_ptr<Shade::KeyValueFile> Shade::KeyValueFile::LoadFile(std::ifstream& fileStream)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();

    std::vector<KeyValuePair> buffer;

    std::string line;
    while (std::getline(fileStream, line))
    {
        size_t delimiter = line.find(':');
        if (delimiter != std::string::npos)
        {
            std::string rawKey = line.substr(0, delimiter);
            uint8_t depth = CountDepth(rawKey);
            std::string key = Shade::StringUtil::trim_copy(rawKey);
            std::string value = delimiter < line.size() - 1 ? Shade::StringUtil::trim_copy(line.substr(delimiter + 2)) : "";

            if (key.empty())
            {
                logger->LogError("Empty key found while parsing key value file, ignoring line...");
                logger->LogInfo(std::string("> ") + line);
                continue;
            }
            if (value.empty())
            {
                logger->LogError("Empty value found while parsing key value file, ignoring line...");
                logger->LogInfo(std::string("> ") + line);
                continue;
            }
            if (value[0] == 'f')
            {
                float floatVal = std::stof(value.substr(1));
                buffer.emplace_back(KeyValuePair{ key, ValueOption::FloatOption(floatVal), depth });
            }
            else if (value[0] == '"')
            {
                std::string stringVal = value.substr(1, value.length() - 2);
                buffer.emplace_back(KeyValuePair{ key, ValueOption::StringOption(stringVal), depth});
            }
            else
            {
                int intVal = std::stoi(value);
                buffer.emplace_back(KeyValuePair{ key, ValueOption::IntOption(intVal), depth});
            }
        }
        else
        {
            size_t delimiter = line.find('>');
            if (delimiter != std::string::npos)
            {
                std::string rawKey = line.substr(0, delimiter);
                uint8_t depth = CountDepth(rawKey);
                std::string key = Shade::StringUtil::trim_copy(rawKey);
                buffer.emplace_back(KeyValuePair{ key, ValueOption::ListOption(), depth});
            }
        }
    }
    return std::make_unique<KeyValueFile>(std::move(buffer));
}

// ======================================
Shade::KeyValueHandle Shade::KeyValueFile::GetContents() const
{
    // Return a handle to the first element in the buffer
    return KeyValueHandle(mBuffer);
}