#include "feature.h"
#include "readwritelock.h"
#include <QThread>
#include <QDebug>
#include <iostream>


Feature::Feature(ReadWriteLock &lock):
    m_rwLock(lock)
{
}

bool Feature::doSomething()
{
    if (!m_rwLock.isLockedForWriting())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    std::cout << "." << std::flush;
    QThread::sleep(1);

    return true;
}

int Feature::getSomething() const
{
    if (!m_rwLock.isLockedForReading())
    {
        qWarning() << "Attempt to access resource with invalid lock";
        return false;
    }

    std::cout << "." << std::flush;
    QThread::sleep(1);

    return 42;
}
