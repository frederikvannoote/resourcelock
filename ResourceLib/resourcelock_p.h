#pragma once

#include <QString>
#include <QList>
#include "singleresourcelock.h"


class ResourceLockPrivate
{
public:
    ResourceLockPrivate(const QString &name);
    virtual ~ResourceLockPrivate();

    bool isValid() const;

private:
    void lock();
    void unlock();

    HANDLE m_mutex = NULL;
    const QString m_name;
    QAtomicInteger<bool> m_isLocked;
};
