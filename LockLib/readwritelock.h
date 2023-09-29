#pragma once

#include "LockLib_global.h"
#include <QSharedPointer>
class ReadWriteLockPrivate;


class LOCKLIB_EXPORT ReadWriteLock
{
public:
    ReadWriteLock(const QString &name);

    enum LockMethod
    {
        READ,
        WRITE
    };
    void lock(const LockMethod &method = READ);
    void unlock();

    bool isLockedForReading() const;
    bool isLockedForWriting() const;

private:
    Q_DECLARE_PRIVATE(ReadWriteLock)
    QSharedPointer<ReadWriteLockPrivate> const d_ptr;
};
