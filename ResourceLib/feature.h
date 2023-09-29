#pragma once

#include "ResourceLib_global.h"
class ResourceLock;
class ReadWriteLock;

class RESOURCELIB_EXPORT Feature
{
public:
    Feature(ReadWriteLock &lock);

    bool doSomething(ResourceLock &lock);
    bool doSomething();

    int getSomething(ResourceLock &lock) const;
    int getSomething() const;

private:
    ReadWriteLock &m_rwLock;
};
