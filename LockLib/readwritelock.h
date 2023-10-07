#pragma once

#include <memory>
#include <string>
class ReadWriteLockPrivate;


class ReadWriteLock
{
public:
    ReadWriteLock(const std::string &name);

    enum LockMethod
    {
        READ,
        WRITE
    };
    void lock(const LockMethod &method = READ);
    void unlock();

    bool isLockedForReading() const;
    bool isLockedForWriting() const;

private:
    std::shared_ptr<ReadWriteLockPrivate> d_ptr;
};
