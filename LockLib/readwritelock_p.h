#pragma once

#include <string>
#include <Windows.h>
#include "readwritelock.h"


/**
 * @brief The ReadWriteLockPrivate class is implemented using a semaphore which
 * represents the number of free locks.
 * Reading consumes 1 lock.
 * Writing consumes all locks (max long).
 *
 * In other words, there can be many parallel readers, but only 1 writer.
 */
class ReadWriteLockPrivate
{
public:
    ReadWriteLockPrivate(const std::string &name);
    ~ReadWriteLockPrivate();

    void lock(const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);
    void unlock();

    bool isLockedForReading() const;
    bool isLockedForWriting() const;

    ReadWriteLock::LockMethod m_method;

private:
    HANDLE m_readLock;
    HANDLE m_writeLock;
    const long m_maxReads;
    const long m_maxWrites;
};
