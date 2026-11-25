#pragma once

class ITaggable
{
public:
    virtual bool IsTagged() const = 0;
};