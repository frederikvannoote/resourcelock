#include "resourcelock.h"
#include "resourcelock_p.h"
#include <QDebug>


ResourceLock::ResourceLock(const QString &name):
    d_ptr(new ResourceLockPrivate(name))
{
}

ResourceLock::ResourceLock(const ResourceLock &other):
    d_ptr(other.d_ptr)
{
}

ResourceLock::~ResourceLock()
{
}

ResourceLock &ResourceLock::operator=(ResourceLock &&other)
{
    operator=(qMove(other));
    return *this;
}

bool ResourceLock::isValid() const
{
    Q_D(const ResourceLock);
    return d->isValid();
}

ResourceLockPrivate::ResourceLockPrivate(const QString &name):
    m_mutex(NULL),
    m_name(name)
{
    const BOOL initialOwner = FALSE;    // thread will not own the mutex after calling CreateMutexA, so no ReleaseMutex call needed when mutex was not locked
    if (name.size() > 0)
    {
        m_mutex = CreateMutexA(nullptr, initialOwner, name.toStdString().c_str());
        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_mutex == NULL)
        {
            qWarning() << "------ failed to create mutex" << name.toStdString().c_str() << m_mutex;
        }
    }

    lock();
}

ResourceLockPrivate::~ResourceLockPrivate()
{
    unlock();
}

bool ResourceLockPrivate::isValid() const
{
    return true;
}

void ResourceLockPrivate::lock()
{
    if (m_mutex)
    {
        DWORD event = WaitForSingleObjectEx(m_mutex, INFINITE, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                qInfo() << "Acquired lock to" << m_name;
                m_isLocked = true;
            }
            else
            {
                qWarning() << "------ problem locking mutex" << m_name << m_mutex << event;
            }
        }
    }
}

void ResourceLockPrivate::unlock()
{
    if (m_mutex  && m_isLocked)
    {
        qInfo() << "Releasing lock to" << m_name << "...";
        BOOL success = ReleaseMutex(m_mutex);
        if (success == TRUE)
        {
            m_isLocked = false;
        }
        else
        {
            qWarning() << "------ failed to unlock mutex" << m_name << m_mutex << "last error" << GetLastError();
        }
    }
}
