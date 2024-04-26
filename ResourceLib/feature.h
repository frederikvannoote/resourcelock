#pragma once

#include <QString>

class ReadWriteLock;
class ReadWriteLocker;

class Feature
{
public:
    Feature(ReadWriteLock &lock);

    bool doSomething(const ReadWriteLocker &rwLocker, const QString &context);

    int getSomething(const ReadWriteLocker &rwLocker, const QString &context) const;

private:
    ReadWriteLock &m_rwLock;
};
