#pragma once

#include "readwritelock.h"
class ReadWriteLockerPrivate;


class ReadWriteLocker
{
public:
    ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);

private:
    std::shared_ptr<ReadWriteLockerPrivate> d_ptr;
};
