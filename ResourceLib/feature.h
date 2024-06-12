#pragma once

#include <QString>

class ReadWriteLock;
class ReadWriteLocker;

class Feature
{
public:
    Feature(ReadWriteLock &lock);

    bool doSomething(const ReadWriteLocker &rwLocker, const QString &context = QString());
    bool doSomething();

    int getSomething(const ReadWriteLocker &rwLocker, const QString &context = QString()) const;
    int getSomething() const;

private:
    ReadWriteLock &m_rwLock;
};
