#include "keyValueFile.h"

#include <cassert>
#include <limits>
#include <vector>

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
        // TODO: Consider logging this as a content error instead, should be easy fix within a file
        assert(index % 2 == 0 && "Depth should have number of spaces divisible by 2");
        return static_cast<uint8_t>(index / 2);
    }

}

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
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mType == ValueType::INT;
}

// ======================================
bool Shade::KeyValueHandle::IsFloat() const
{
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mType == ValueType::FLOAT;
}

// ======================================
bool Shade::KeyValueHandle::IsString() const
{
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mType == ValueType::STRING;
}

// ======================================
bool Shade::KeyValueHandle::IsList() const
{
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mType == ValueType::LIST;
}

// ======================================
const std::string& Shade::KeyValueHandle::GetKey() const
{
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mKey;
}

// ======================================
int Shade::KeyValueHandle::GetInt() const
{
    assert(IsInt() && "Expected int type to access int value");
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mInt;
}

// ======================================
float Shade::KeyValueHandle::GetFloat() const
{
    assert(IsFloat() && "Expected float type to access float value");
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mFloat;
}

// ======================================
const std::string& Shade::KeyValueHandle::GetString() const
{
    assert(IsString() && "Expected string type to access string value");
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    return mBufferRef[mIndex].mValue.mString;
}

// ======================================
Shade::KeyValueHandle Shade::KeyValueHandle::GetListHead() const
{
    assert(IsList() && "Expected list type to access list value");
    assert(mIndex < mBufferRef.size() && "Index access should be within buffer bounds");
    // TODO: More error checking on the format
    //  - If the next item is not exactly 1 depth below, should return an invalid handle
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
            else 
            {
                logger->LogWarning("Unable to parse line in key value file, ignoring line...");
                logger->LogInfo(std::string("> ") + line);
            }
        }
    }
    return std::make_unique<KeyValueFile>(std::move(buffer));
}

// ======================================
bool Shade::KeyValueFile::SaveFile(std::ofstream& fileStream) const
{
    for (const KeyValuePair& pair : mBuffer)
    {
        std::string line = StringUtil::repeat("  ", pair.mDepth) + pair.mKey;
        if (pair.mValue.mType == ValueType::LIST)
        {
            line += " >";
        }
        else 
        {
            line += " : ";
            if (pair.mValue.mType == ValueType::INT)
            {
                line += std::to_string(pair.mValue.mInt);
            }
            if (pair.mValue.mType == ValueType::FLOAT)
            {
                line += 'f' + std::to_string(pair.mValue.mFloat);
            }
            if (pair.mValue.mType == ValueType::STRING)
            {
                line += '"' + pair.mValue.mString + '"';
            }
        }
        line += '\n';
        fileStream << line;
    }

    return true;
}

// ======================================
Shade::KeyValueHandle Shade::KeyValueFile::GetContents() const
{
    // Return a handle to the first element in the buffer
    return KeyValueHandle(mBuffer);
}

// ======================================
Shade::KeyValueFile Shade::KeyValueFile::CreateFileForWrite()
{
    return {};
}

// ======================================
void Shade::KeyValueFile::EndWriteNewFile()
{
    assert(mCurrentDepth == 0 && "Depth should be 0 once key value file is done writing");
}

// ======================================
void Shade::KeyValueFile::AddIntEntry(const std::string& key, int value)
{
    mBuffer.emplace_back(KeyValuePair{ key, ValueOption::IntOption(value), mCurrentDepth });
}

// ======================================
void Shade::KeyValueFile::AddFloatEntry(const std::string& key, float value)
{
    mBuffer.emplace_back(KeyValuePair{ key, ValueOption::FloatOption(value), mCurrentDepth });
}

// ======================================
void Shade::KeyValueFile::AddStringEntry(const std::string& key, const std::string& value)
{
    mBuffer.emplace_back(KeyValuePair{ key, ValueOption::StringOption(value), mCurrentDepth });
}

// ======================================
void Shade::KeyValueFile::PushList(const std::string& key)
{
    mBuffer.emplace_back(KeyValuePair{ key, ValueOption::ListOption(), mCurrentDepth });
    mCurrentDepth++;
}

// ======================================
void Shade::KeyValueFile::PopList()
{
    assert(mCurrentDepth > 0 && "Tried to pop list when depth is already 0");
    mCurrentDepth--;
}
