#include "readwritelock.h"
#include "readwritelock_p.h"
#include <limits.h>
#include <iostream>


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
    m_name(name),
    m_lock(nullptr),
    m_maxReads(10 /* LONG_MAX */)
{
    if (name.size() > 0)
    {
        m_lock = CreateSemaphoreA(nullptr, m_maxReads, m_maxReads, name.c_str());

        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_lock == NULL)
        {
            std::cerr << "Failed to create semaphore " << name << " " <<
                m_lock << " " << GetLastError();
        }
    }
}

ReadWriteLockPrivate::~ReadWriteLockPrivate()
{
    if (m_lock)
        CloseHandle(m_lock);
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
                }
                else
                {
                    std::cerr << "Problem locking mutex " << /*m_name <<*/ m_lock << " " << event;
                }
            }
        }
    }
}

void ReadWriteLockPrivate::unlock()
{
    if (m_lock)
    {
        // qInfo() << "Releasing lock to" << m_name << "...";
        const long max = (m_method == ReadWriteLock::READ) ? 1:m_maxReads;
        BOOL success = ReleaseSemaphore(m_lock, max, nullptr);
        if (success == TRUE)
        {
        }
        else
        {
            std::cerr << "Failed to unlock mutex " << /*m_name <<*/ m_lock << " last error " << GetLastError();
        }
    }
}

bool ReadWriteLockPrivate::isLockedForReading() const
{
    bool result = false;
    HANDLE handle = CreateSemaphoreA(nullptr, m_maxReads, m_maxReads, m_name.c_str());

    if (handle == NULL) {
        std::cerr << "Failed to create mutex: " << GetLastError() << std::endl;
        return false;
    }

    DWORD dwWaitResult = WaitForSingleObject(handle, 0);

    switch (dwWaitResult) {
    case WAIT_OBJECT_0:
//        std::cout << "Mutex is available, your thread has locked it." << std::endl;
        ReleaseSemaphore(handle, 1, NULL);
        result = true;
        break;

    case WAIT_TIMEOUT:
        std::cout << "Mutex is currently owned by another thread." << std::endl;
        break;

    case WAIT_ABANDONED:
        std::cerr << "Mutex was abandoned." << std::endl;
        break;

    case WAIT_FAILED:
        std::cerr << "Wait for mutex failed: " << GetLastError() << std::endl;
        break;

    default:
        std::cerr << "Unknown wait result." << std::endl;
        break;
    }

    CloseHandle(handle);

    return result;
}

bool ReadWriteLockPrivate::isLockedForWriting() const
{
    bool result = false;
    HANDLE handle = CreateSemaphoreA(nullptr, m_maxReads, m_maxReads, m_name.c_str());

    if (handle == NULL) {
        std::cerr << "Failed to create mutex: " << GetLastError() << std::endl;
        return false;
    }

    DWORD dwWaitResult = WaitForSingleObject(handle, 0);

    switch (dwWaitResult) {
    case WAIT_OBJECT_0:
//        std::cout << "Mutex is available, your thread has locked it." << std::endl;
        ReleaseSemaphore(handle, m_maxReads, NULL);
        result = true;
        break;

    case WAIT_TIMEOUT:
        std::cout << "Mutex is currently owned by another thread." << std::endl;
        break;

    case WAIT_ABANDONED:
        std::cerr << "Mutex was abandoned." << std::endl;
        break;

    case WAIT_FAILED:
        std::cerr << "Wait for mutex failed: " << GetLastError() << std::endl;
        break;

    default:
        std::cerr << "Unknown wait result." << std::endl;
        break;
    }

    CloseHandle(handle);

    return result;
}
