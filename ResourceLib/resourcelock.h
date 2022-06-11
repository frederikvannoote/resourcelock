#pragma once

#include "ResourceLib_global.h"
#include <QSharedPointer>
class ResourceLockPrivate;

class RESOURCELIB_EXPORT ResourceLock
{
public:
    ResourceLock(const QString &name);
    ResourceLock(const ResourceLock &other);
    ResourceLock(const ResourceLock &&other);
    virtual ~ResourceLock();

    ResourceLock &operator=(ResourceLock &other);
    ResourceLock &operator=(ResourceLock &&other);

    bool isValid() const;

private:
    Q_DECLARE_PRIVATE(ResourceLock)
    QSharedPointer<ResourceLockPrivate> const d_ptr;
};
