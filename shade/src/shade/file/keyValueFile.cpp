#include "keyValueFile.h"

#include <vector>

#include "shade/common/stringUtil.h"

// ======================================
Shade::KeyValueFile::KeyValueFile(KeyValueList&& contents)
    : mContents(std::move(contents))
{

}

// ======================================
std::unique_ptr<Shade::KeyValueFile> Shade::KeyValueFile::LoadFile(std::ifstream& fileStream)
{
    KeyValueList list;

    std::string line;
    while (std::getline(fileStream, line))
    {
        size_t delimiter = line.find(':');
        if (delimiter != std::string::npos)
        {
            std::string key = Shade::StringUtil::trim_copy(line.substr(0, delimiter));
            std::string value = Shade::StringUtil::trim_copy(line.substr(delimiter + 2));

            if (key.empty())
            {
                // TODO: Error
            }
            if (value.empty())
            {
                // TODO: Error
            }
            if (value[0] == 'f')
            {
                float floatVal = std::stof(value.substr(1));
                list.emplace(key, ValueOption::FloatOption(floatVal));
            }
            else if (value[0] == '"')
            {
                std::string stringVal = value.substr(1, value.length() - 2);
                list.emplace(key, ValueOption::StringOption(stringVal));
            }
            else
            {
                int intVal = std::stoi(value);
                list.emplace(key, ValueOption::IntOption(intVal));
            }
        }
        else
        {
            // TODO: List parsing?
        }
    }
    return std::make_unique<KeyValueFile>(std::move(list));
}

// ======================================
const Shade::KeyValueFile::KeyValueList& Shade::KeyValueFile::GetContents() const
{
    return mContents;
}