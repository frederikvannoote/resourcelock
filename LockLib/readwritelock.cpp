#include "readwritelock.h"
#include "readwritelock_p.h"
#include <limits.h>
#include <iostream>
#include <QDebug>


ReadWriteLock::ReadWriteLock(const std::string &name):
    d_ptr(new ReadWriteLockPrivate(name))
{
}

void ReadWriteLock::lock(const LockMethod &method)
{
    return d_ptr->lock(method);
}

void ReadWriteLock::unlock(const LockMethod &method)
{
    return d_ptr->unlock(method);
}

ReadWriteLockPrivate::ReadWriteLockPrivate(const std::string &name):
    m_readLock(nullptr),
    m_writeLock(nullptr),
    m_maxReads(10 /* LONG_MAX */),
    m_maxWrites(1)
{
    if (name.size() > 0)
    {
        // Create read semaphore
        std::string readName = name + "Read";
        m_readLock = CreateSemaphoreA(nullptr, m_maxReads, m_maxReads, readName.c_str());

        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_readLock == NULL)
        {
            qWarning() << "--- Failed to create semaphore " << name << " " <<
                m_readLock << " " << GetLastError();
        }
        else
        {
            qInfo() << "--- Created read semaphore" << readName;
        }

        // Create write semaphore
        std::string writeName = name + "Write";
        m_writeLock = CreateSemaphoreA(nullptr, m_maxWrites, m_maxWrites, writeName.c_str());

        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_writeLock == NULL)
        {
            qWarning() << "--- Failed to create semaphore " << name << " " <<
                m_writeLock << " " << GetLastError();
        }
        else
        {
            qInfo() << "--- Created write semaphore" << writeName;
        }
    }
}

ReadWriteLockPrivate::~ReadWriteLockPrivate()
{
    qInfo() << "--- destructor ReadWriteLockPrivate";
    if (m_readLock)
    {
        qInfo() << "--- closing read lock handle";
        CloseHandle(m_readLock);
    }
    if (m_writeLock)
    {
        qInfo() << "--- closing write lock handle";
        CloseHandle(m_writeLock);
    }
}

void ReadWriteLockPrivate::lock(const ReadWriteLock::LockMethod &method)
{
    if (method == ReadWriteLock::WRITE)
    {
        qInfo() << "--- Attempting to acquire write lock" << m_writeLock;
        // Acquire write lock
        DWORD event = WaitForSingleObjectEx(m_writeLock, INFINITE, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                qInfo() << "--- Acquired write lock" << m_writeLock;
            }
            else
            {
                qWarning() << "--- Problem locking mutex " << m_writeLock << " " << event;
                return;
            }
        }

        // Acquire all read locks to avoid read access during writing
        for (long i=0; i<m_maxReads; i++)
        {
            qInfo() << "--- Attempting to acquire read lock" << i << m_readLock << "after acquiring write lock";
            DWORD event = WaitForSingleObjectEx(m_readLock, INFINITE, FALSE);
            {
                if (event == WAIT_OBJECT_0)
                {
                    qInfo() << "--- Acquired read lock" << i << "after acquiring write lock";
                }
                else
                {
                    qWarning() << "--- Problem locking mutex " << m_readLock << " " << event;
                    return;
                }
            }
        }
    }
    else
    {
        qInfo() << "--- Attempting to acquire read lock" << m_readLock;
        // Acquire read lock
        DWORD event = WaitForSingleObjectEx(m_readLock, INFINITE, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                qInfo() << "--- Acquired read lock" << m_readLock;
            }
            else
            {
                qWarning() << "--- Problem locking mutex " << m_readLock << " " << event;
            }
        }
    }
}

void ReadWriteLockPrivate::unlock(const ReadWriteLock::LockMethod &method)
{
    if (m_readLock && m_writeLock)
    {
        if (method == ReadWriteLock::WRITE)
        {
            qInfo() << "--- Attempting to release all read locks" << m_readLock;
            // Release all read locks
            BOOL success = ReleaseSemaphore(m_readLock, m_maxReads, nullptr);
            if (success == TRUE)
            {
                qInfo() << "--- released" << m_maxReads << "read locks" << m_readLock;
            }
            else
            {
                qWarning() << "--- Failed to unlock mutex " << m_readLock << " last error " << GetLastError();
            }

            // Release write lock
            qInfo() << "--- Attempting to release write lock" << m_writeLock;
            success = ReleaseSemaphore(m_writeLock, 1, nullptr);
            if (success == TRUE)
            {
                qInfo() << "--- released write lock" << m_writeLock;
            }
            else
            {
                qWarning() << "--- Failed to unlock writeLock mutex " << m_writeLock << " last error " << GetLastError();
            }
        }
        else
        {
            qInfo() << "--- Attempting to release read lock" << m_readLock;
            BOOL success = ReleaseSemaphore(m_readLock, 1, nullptr);
            if (success == TRUE)
            {
                qInfo() << "--- released read lock";
            }
            else
            {
                qWarning() << "--- Failed to unlock readLock mutex " << m_readLock << " last error " << GetLastError();
            }
        }
    }
}
