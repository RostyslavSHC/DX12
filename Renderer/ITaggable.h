#pragma once

class ITaggable
{
public:
    // Constructors/Destructors
    virtual ~ITaggable() = default;

    // Accessors
    virtual bool IsTagged() const = 0;
};