#pragma once

#include "LockLib_global.h"
#include "readwritelock.h"
#include <QString>
class ReadWriteLockerPrivate;


class LOCKLIB_EXPORT ReadWriteLocker
{
public:
    ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ, const QString& context = QString());

    bool isLockedForWriting() const;
    bool isLockedForReading() const;

private:
    std::shared_ptr<ReadWriteLockerPrivate> d_ptr;
};
