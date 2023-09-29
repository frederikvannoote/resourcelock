#pragma once

#include "LockLib_global.h"
#include "readwritelock.h"
class ReadWriteLockerPrivate;


class LOCKLIB_EXPORT ReadWriteLocker
{
public:
    ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);

private:
    std::shared_ptr<ReadWriteLockerPrivate> d_ptr;
};
