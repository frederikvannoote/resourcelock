#include "resourcelockwatcher.h"
#include <QDebug>


ResourceLockWatcher::ResourceLockWatcher(const QString &name, QObject *parent):
    QObject(parent),
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
}

bool ResourceLockWatcher::isLocked() const
{
    return false;
}
