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

ResourceLock::ResourceLock(const ResourceLock &&other):
    d_ptr(qMove(other.d_ptr))
{
}

ResourceLock::~ResourceLock()
{
}

ResourceLock &ResourceLock::operator=(ResourceLock &other)
{
    if (this != &other) {
        d_ptr = other.d_ptr;
    }
    return *this;
}

ResourceLock &ResourceLock::operator=(ResourceLock &&other)
{
    if (this != &other) {
        qSwap(d_ptr, other.d_ptr);
    }
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

}

void ResourceLockPrivate::unlock()
{

}


