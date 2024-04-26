#include "feature.h"
#include "readwritelock.h"
#include "readwritelocker.h"
#include <QThread>
#include <QDebug>
#include <iostream>


Feature::Feature(ReadWriteLock &lock):
    m_rwLock(lock)
{
}

bool Feature::doSomething(const ReadWriteLocker &rwLocker, const QString &context)
{
    if (!rwLocker.isLockedForWriting())
    {
        qWarning() << "Write lock was not acquired for resource";
        return false;
    }

    qInfo() << context << "is writing";

    // QThread::msleep(20);

    return true;
}

int Feature::getSomething(const ReadWriteLocker &rwLocker, const QString &context) const
{
    if (!rwLocker.isLockedForReading())
    {
        qWarning() << "Read lock was not acquired for resource";
        return false;
    }

    qInfo() << context << "is reading";

    // QThread::msleep(10);

    return 42;
}
