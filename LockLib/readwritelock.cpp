#include "readwritelock.h"
#include "readwritelock_p.h"
#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <fileapi.h>


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
    return d_ptr->isLocked() && d_ptr->method() == ReadWriteLock::READ;
}

bool ReadWriteLock::isLockedForWriting() const
{
    return d_ptr->isLocked() && d_ptr->method() == ReadWriteLock::WRITE;
}

ReadWriteLockPrivate::ReadWriteLockPrivate(const std::string &name):
    m_name(name),
    m_method(ReadWriteLock::READ),
    m_file(nullptr)
{
}

ReadWriteLockPrivate::~ReadWriteLockPrivate()
{
    unlock();
}

void ReadWriteLockPrivate::lock(const ReadWriteLock::LockMethod &method)
{
    m_method = method;

    std::cout << "Acquiring the lock...";
    while (!m_file)
    {
        const std::wstring name = std::wstring(m_name.begin(), m_name.end());

        if (method == ReadWriteLock::LockMethod::WRITE)
        {
            m_file = CreateFile(name.c_str(),           // name of the write
                                GENERIC_WRITE,          // open for writing
                                0,                      // do not share
                                NULL,                   // default security
                                OPEN_ALWAYS,            // if it does not exist, create it
                                FILE_ATTRIBUTE_NORMAL,  // normal file
                                NULL);                  // no attr. template
        }
        else
        {
            m_file = CreateFile(name.c_str(),           // name of the write
                                GENERIC_READ,           // open for reading
                                FILE_SHARE_READ,        // can share
                                NULL,                   // default security
                                OPEN_ALWAYS,            // if it does not exist, create it
                                FILE_ATTRIBUTE_NORMAL,  // normal file
                                NULL);                  // no attr. template
        }

        if (m_file == INVALID_HANDLE_VALUE)
        {
            std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            m_file = nullptr;
        }
    }
    std::cout << "[locked]" << std::endl;
}

void ReadWriteLockPrivate::unlock()
{
    if (m_file)
    {
        CloseHandle(m_file);
        m_file = nullptr;
    }
}

bool ReadWriteLockPrivate::isLocked() const
{
    return m_file != nullptr;
}

ReadWriteLock::LockMethod ReadWriteLockPrivate::method() const
{
    return m_method;
}
