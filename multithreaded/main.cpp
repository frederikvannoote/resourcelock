#include <QCoreApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include "resource.h"
//#include "cancellationrequest.h"
#include "readwritelock.h"
#include "readwritelocker.h"

void readSomething(const QString &name, QSharedPointer<Resource> resource, int priority)
{
    QThread::msleep(1000);

    qDebug() << name << "Starting to read something with priority" << priority << "...";

    // Acquire lock
    qDebug() << name << "Acquiring the read lock...";
    ReadWriteLocker locker(resource->rwLock(), ReadWriteLock::LockMethod::READ, name);
    qDebug() << name << "Acquired the read lock.";

    // Read the resource with intermediate checks if cancellation was requested.
    // std::cout << name.toStdString() << " Reading from the resource";
    for (int i=0; i<10; i++)
    {
        qInfo() << name.toStdString() << "reading from resource, i" << i;
            resource->features().at(0)->getSomething(locker, name);
    }
}

void writeSomething(const QString &name, QSharedPointer<Resource> resource, int priority)
{
    qDebug() << name << "Starting to write something with priority" << priority << "...";

    // Acquire lock
    qDebug() << name << "Acquiring the write lock...";
    ReadWriteLocker locker(resource->rwLock(), ReadWriteLock::LockMethod::WRITE, name);
    qDebug() << name << "Acquired the write lock.";

    // Write to the resource with intermediate checks if cancellation was requested.
    // std::cout << name.toStdString() << " Writing to the resource";
    for (int i=0; i<10; i++)
    {
            qInfo() << name.toStdString() << "writing to the resource, i" << i;
            if (!resource->features().at(0)->doSomething(locker, name))
                qWarning() << name.toStdString() << " accesses without lock.";
    }
    QThread::msleep(500);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedPointer<Resource> resource(new Resource("PreciousResource"));

    {
        QFutureSynchronizer<void> synchronizer;

        for(int i = 0; i < 5; i++)
        {
            synchronizer.addFuture(QtConcurrent::run(&writeSomething, QString("writer %1").arg(i), resource, 1));
            synchronizer.addFuture(QtConcurrent::run(&readSomething, QString("reader %1").arg(i), resource, 1));
        }
   }

    return 0;
}
