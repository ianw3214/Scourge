#include "textFile.h"

// ======================================
Shade::TextFile::TextFile(std::vector<std::string>&& contents)
    : mContents(std::move(contents))
{

}

// ======================================
std::unique_ptr<Shade::TextFile> Shade::TextFile::LoadFile(std::ifstream& fileStream)
{
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fileStream, line))
    {
        lines.emplace_back(line);
    }
    return std::make_unique<TextFile>(std::move(lines));
}

// ======================================
const std::vector<std::string>& Shade::TextFile::GetContents() const
{
    return mContents;
}