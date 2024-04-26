#pragma once

#include "LockLib_global.h"
#include "readwritelock.h"
#include <QString>
class ReadWriteLockerPrivate;


class LOCKLIB_EXPORT ReadWriteLocker
{
public:
    ReadWriteLocker(ReadWriteLock &lock, const QString& context, const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);

    bool isLockedForWriting() const;
    bool isLockedForReading() const;

private:
    std::shared_ptr<ReadWriteLockerPrivate> d_ptr;
};
