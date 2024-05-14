#include "editorBase.h"

// ======================================
Shade::EditorBase::EditorBase(const std::string& name)
    : mName(name)
{

}

// ======================================
Shade::EditorBase::~EditorBase() = default;

// ======================================
const std::string& Shade::EditorBase::GetName() const
{
    return mName;
}