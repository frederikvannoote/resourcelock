#pragma once

#include <QString>
#include <QAtomicInteger>
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
    ReadWriteLockPrivate(const QString &name);

    void lock(const ReadWriteLock::LockMethod &method = ReadWriteLock::LockMethod::READ);
    void unlock();

    ReadWriteLock::LockMethod m_method;
    bool m_isLocked;

private:
    HANDLE m_lock;
    const long m_maxReads;
};
