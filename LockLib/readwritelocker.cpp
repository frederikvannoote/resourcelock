#include "readwritelocker.h"
#include "readwritelocker_p.h"
#include "readwritelock.h"
#include <QDebug>


ReadWriteLocker::ReadWriteLocker(ReadWriteLock &lock, const QString& context, const ReadWriteLock::LockMethod &method):
    d_ptr(new ReadWriteLockerPrivate(lock, context, method))
{
}

bool ReadWriteLocker::isLockedForReading() const
{
    qInfo() << d_ptr->m_context << "is locked for reading";
    return true;
}

bool ReadWriteLocker::isLockedForWriting() const
{
    if (d_ptr->m_method == ReadWriteLock::LockMethod::WRITE)
    {
        qInfo() << d_ptr->m_context << "is locked for writing";
        return true;
    }
    else
    {
        qInfo() << d_ptr->m_context << "is not locked for writing because lock method is READ";
        return false;
    }
    // return (d_ptr->m_method == ReadWriteLock::LockMethod::WRITE);
}

ReadWriteLockerPrivate::ReadWriteLockerPrivate(ReadWriteLock &lock, const QString &context, const ReadWriteLock::LockMethod &method):
    m_lock(lock),
    m_method(method),
    m_context(context)
{
    QString logString = (method == ReadWriteLock::READ) ? "read" : "write";
    qInfo() << context << "attempting to lock with method" << logString;

    lock.lock(method);

    qInfo() << context << "succeeded to lock with method" << logString;
}

ReadWriteLockerPrivate::~ReadWriteLockerPrivate()
{
    QString logString = (m_method == ReadWriteLock::READ) ? "read" : "write";
    qInfo() << m_context << "attempting to unlock with method" << logString;

    m_lock.unlock(m_method);

    qInfo() << m_context << "succeeded to unlock with method" << logString;
}
