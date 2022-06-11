#pragma once

#include "ResourceLib_global.h"
#include "resourcelock.h"


class RESOURCELIB_EXPORT Resource
{
public:
    Resource(const QString &name);

    ResourceLock lock(int priority = 0) const;

    bool doSomething();
    bool doSomething(ResourceLock &lock);

    int getSomething() const;
    int getSomething(ResourceLock &lock) const;

private:
    const QString m_name;
};
