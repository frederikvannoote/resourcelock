#pragma once

#include "readwritelock.h"


class ReadWriteLockerPrivate
{
public:
    ReadWriteLockerPrivate(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method);
    virtual ~ReadWriteLockerPrivate();

    ReadWriteLock &m_lock;
};
