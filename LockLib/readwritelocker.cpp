#include "readwritelocker.h"
#include "readwritelocker_p.h"
#include "readwritelock.h"
#include <QDebug>


ReadWriteLocker::ReadWriteLocker(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method, const QString& context):
    d_ptr(new ReadWriteLockerPrivate(lock, method, context))
{
}

bool ReadWriteLocker::isLockedForReading() const
{
    qInfo() << d_ptr->m_context << "is locked for reading";
    return true;
}

bool ReadWriteLocker::isLockedForWriting() const
{
    qInfo() << d_ptr->m_context << ((d_ptr->m_method == ReadWriteLock::LockMethod::WRITE) ? "is locked for writing" : "is not locked for writing because lock method is READ");
    return (d_ptr->m_method == ReadWriteLock::LockMethod::WRITE);
}

ReadWriteLockerPrivate::ReadWriteLockerPrivate(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method, const QString &context):
    m_lock(lock),
    m_method(method),
    m_context(context)
{
    qInfo() << context << "attempting to lock with method" << ((method == ReadWriteLock::READ) ? "read" : "write");

    lock.lock(method);

    qInfo() << context << "succeeded to lock with method" << ((method == ReadWriteLock::READ) ? "read" : "write");
}

ReadWriteLockerPrivate::~ReadWriteLockerPrivate()
{
    qInfo() << m_context << "attempting to unlock with method" << ((m_method == ReadWriteLock::READ) ? "read" : "write");

    m_lock.unlock(m_method);

    qInfo() << m_context << "succeeded to unlock with method" << ((m_method == ReadWriteLock::READ) ? "read" : "write");
}
