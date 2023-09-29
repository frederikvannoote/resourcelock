#include "readwritelock.h"
#include "readwritelock_p.h"
#include <QDebug>
#include <limits.h>


ReadWriteLock::ReadWriteLock(const QString &name):
    d_ptr(new ReadWriteLockPrivate(name))
{
}

void ReadWriteLock::lock(const LockMethod &method)
{
    Q_D(ReadWriteLock);
    return d->lock(method);
}

void ReadWriteLock::unlock()
{
    Q_D(ReadWriteLock);
    return d->unlock();
}

bool ReadWriteLock::isLockedForReading() const
{
    Q_D(const ReadWriteLock);
    return d->m_isLocked && d->m_method == ReadWriteLock::READ;
}

bool ReadWriteLock::isLockedForWriting() const
{
    Q_D(const ReadWriteLock);
    return d->m_isLocked && d->m_method == ReadWriteLock::WRITE;
}

ReadWriteLockPrivate::ReadWriteLockPrivate(const QString &name):
    m_method(ReadWriteLock::READ),
    m_isLocked(false),
    m_lock(nullptr),
    m_maxReads(10 /* LONG_MAX */)
{
    if (name.size() > 0)
    {
        m_lock = CreateSemaphoreA(nullptr, m_maxReads, m_maxReads, name.toStdString().c_str());

        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_lock == NULL)
        {
            qWarning() << "------ failed to create semaphore" << name.toStdString().c_str() <<
                m_lock << GetLastError();
        }
    }
}

void ReadWriteLockPrivate::lock(const ReadWriteLock::LockMethod &method)
{
    if (m_lock)
    {
        const long max = (method == ReadWriteLock::READ) ? 1:m_maxReads;
        for (long i=0; i<max; i++)
        {
            DWORD event = WaitForSingleObjectEx(m_lock, INFINITE, FALSE);
            {
                if (event == WAIT_OBJECT_0)
                {
                    // qInfo() << "Acquired lock to" << m_name;
                    m_method = method;
                    m_isLocked = true;
                }
                else
                {
                    qWarning() << "------ problem locking mutex" << /*m_name <<*/ m_lock << event;
                }
            }
        }
    }
}

void ReadWriteLockPrivate::unlock()
{
    if (m_lock /* && m_isLocked*/)
    {
        // qInfo() << "Releasing lock to" << m_name << "...";
        const long max = (m_method == ReadWriteLock::READ) ? 1:m_maxReads;
        BOOL success = ReleaseSemaphore(m_lock, max, nullptr);
        if (success == TRUE)
        {
            m_isLocked = false;
        }
        else
        {
            qWarning() << "------ failed to unlock mutex" << /*m_name <<*/ m_lock << "last error" << GetLastError();
        }
    }
}
