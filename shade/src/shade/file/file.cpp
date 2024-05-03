#include "file.h"

// ======================================
Shade::File::File(std::vector<std::string>&& contents)
    : mContents(std::move(contents))
{

}

// ======================================
const std::vector<std::string>& Shade::File::GetContents() const
{
    return mContents;
}