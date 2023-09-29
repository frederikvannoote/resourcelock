#pragma once

#include "LockLib_global.h"
#include "readwritelock.h"
class ReadWriteLockerPrivate;


class LOCKLIB_EXPORT ReadWriteLocker
{
public:
    ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);

private:
    Q_DECLARE_PRIVATE(ReadWriteLocker)
    QSharedPointer<ReadWriteLockerPrivate> const d_ptr;
};
