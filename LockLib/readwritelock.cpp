#include "readwritelock.h"
#include "readwritelock_p.h"
#include <limits.h>
#include <iostream>

// Define thread-local storage (TLS) variable
__declspec(thread) bool locked = false;


ReadWriteLock::ReadWriteLock(const std::string &name):
    d_ptr(new ReadWriteLockPrivate(name))
{
}

void ReadWriteLock::lock(const LockMethod &method)
{
    return d_ptr->lock(method);
}

void ReadWriteLock::unlock()
{
    return d_ptr->unlock();
}

bool ReadWriteLock::isLockedForReading() const
{
    return d_ptr->isLockedForReading();
}

bool ReadWriteLock::isLockedForWriting() const
{
    return d_ptr->isLockedForWriting();
}

ReadWriteLockPrivate::ReadWriteLockPrivate(const std::string &name):
    m_method(ReadWriteLock::READ),
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
            std::cerr << "Failed to create semaphore " << name << " " <<
                m_readLock << " " << GetLastError();
        }

        // Create write semaphore
        std::string writeName = name + "Write";
        m_writeLock = CreateSemaphoreA(nullptr, m_maxWrites, m_maxWrites, writeName.c_str());

        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_writeLock == NULL)
        {
            std::cerr << "Failed to create semaphore " << name << " " <<
                m_writeLock << " " << GetLastError();
        }
    }
}

ReadWriteLockPrivate::~ReadWriteLockPrivate()
{
    if (m_readLock)
        CloseHandle(m_readLock);
    if (m_writeLock)
        CloseHandle(m_writeLock);
}

void ReadWriteLockPrivate::lock(const ReadWriteLock::LockMethod &method)
{
    if (method == ReadWriteLock::WRITE)
    {
        // Acquire write lock
        DWORD event = WaitForSingleObjectEx(m_writeLock, INFINITE, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                // qInfo() << "Acquired lock to" << m_name;
            }
            else
            {
                std::cerr << "Problem locking mutex " << /*m_name <<*/ m_writeLock << " " << event;
                return;
            }
        }

        // Acquire all read locks to avoid read access during writing
        for (long i=0; i<m_maxReads; i++)
        {
            DWORD event = WaitForSingleObjectEx(m_readLock, INFINITE, FALSE);
            {
                if (event == WAIT_OBJECT_0)
                {
                    // qInfo() << "Acquired lock to" << m_name;
                }
                else
                {
                    std::cerr << "Problem locking mutex " << /*m_name <<*/ m_readLock << " " << event;
                    return;
                }
            }
        }

        m_method = method;
        locked = true;
    }
    else
    {
        // Acquire read lock
        DWORD event = WaitForSingleObjectEx(m_readLock, INFINITE, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                // qInfo() << "Acquired lock to" << m_name;
                m_method = method;
                locked = true;
            }
            else
            {
                std::cerr << "Problem locking mutex " << /*m_name <<*/ m_readLock << " " << event;
            }
        }
    }
}

void ReadWriteLockPrivate::unlock()
{
    if (m_readLock && m_writeLock && locked)
    {
        // qInfo() << "Releasing lock to" << m_name << "...";
        if (m_method == ReadWriteLock::WRITE)
        {
            // Release all read locks
            BOOL success = ReleaseSemaphore(m_readLock, m_maxReads, nullptr);
            if (success == TRUE)
            {
            }
            else
            {
                std::cerr << "Failed to unlock mutex " << /*m_name <<*/ m_readLock << " last error " << GetLastError();
            }

            // Release write lock
            success = ReleaseSemaphore(m_writeLock, 1, nullptr);
            if (success == TRUE)
            {
            }
            else
            {
                std::cerr << "Failed to unlock mutex " << /*m_name <<*/ m_writeLock << " last error " << GetLastError();
            }

            locked = false;
        }
        else
        {
            BOOL success = ReleaseSemaphore(m_readLock, 1, nullptr);
            if (success == TRUE)
            {
                locked = false;
            }
            else
            {
                std::cerr << "Failed to unlock mutex " << /*m_name <<*/ m_readLock << " last error " << GetLastError();
            }
        }
    }
}

bool ReadWriteLockPrivate::isLockedForReading() const
{
    return m_method == ReadWriteLock::READ && locked;
}

bool ReadWriteLockPrivate::isLockedForWriting() const
{
    return m_method == ReadWriteLock::WRITE && locked;
}
