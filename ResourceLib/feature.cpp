#include "feature.h"
#include "resourcelock.h"
#include "readwritelock.h"
#include <QThread>
#include <QDebug>
#include <iostream>


Feature::Feature(ReadWriteLock &lock):
    m_rwLock(lock)
{
}

bool Feature::doSomething(ResourceLock &lock)
{
    if (!lock.isValid())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    qDebug() << "Do something";
    QThread::sleep(10);
    qDebug() << "Done something";

    return true;
}

bool Feature::doSomething()
{
    if (!m_rwLock.isLockedForWriting())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    std::cout << ".";
    QThread::sleep(1);

    return true;
}

int Feature::getSomething(ResourceLock &lock) const
{
    if (!lock.isValid())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    std::cout << ".";
    QThread::sleep(1);

    return 42;
}

int Feature::getSomething() const
{
    if (!m_rwLock.isLockedForReading())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    std::cout << ".";
    QThread::sleep(1);

    return 42;
}
