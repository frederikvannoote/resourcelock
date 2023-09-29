#include "readwritelocker.h"
#include "readwritelocker_p.h"
#include "readwritelock.h"


ReadWriteLocker::ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method):
    d_ptr(new ReadWriteLockerPrivate(lock, method))
{
}

ReadWriteLockerPrivate::ReadWriteLockerPrivate(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method):
    m_lock(lock)
{
    lock.lock(method);
}

ReadWriteLockerPrivate::~ReadWriteLockerPrivate()
{
    m_lock.unlock();
}
