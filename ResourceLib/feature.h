#pragma once

#include "ResourceLib_global.h"
class ReadWriteLock;

class RESOURCELIB_EXPORT Feature
{
public:
    Feature(ReadWriteLock &lock);

    bool doSomething();

    int getSomething() const;

private:
    ReadWriteLock &m_rwLock;
};
