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
    qDebug() << name << "Starting to read something with priority" << priority << "...";

    // Send cancellation request to all lower levels.
//    QVector<QSharedPointer<CancellationRequest>> cancellationRequests;
//    for (int prio = priority+1; prio <= QThread::InheritPriority; prio++)
//    {
//        QSharedPointer<CancellationRequest> r(new CancellationRequest(QString(resource->name() + QStringLiteral("-cancellation-") + QString::number(prio)).toStdString()));
//        cancellationRequests.append(r);

//        qInfo() << "[prio" << priority << "] Sending cancellation request to prio" << prio;
//        r->request();
//    }

    // Acquire lock
    qDebug() << name << "Acquiring the read lock...";
    ReadWriteLocker lock(resource->rwLock(), ReadWriteLock::LockMethod::READ);
    qDebug() << name << "Acquired the read lock.";

//    CancellationRequest cancellation(QString(resource->name() + QStringLiteral("-cancellation-") + QString::number(priority)).toStdString());

    // Read the resource with intermediate checks if cancellation was requested.
    std::cout << name.toStdString() << " Reading from the resource";
    for (int i=0; i<10; i++)
    {
//        if (!cancellation.isRequested())
            resource->features().at(0)->getSomething();
//        else
//        {
//            qDebug() << name << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
//            break;
//        }
    }
    std::cout << std::endl;
}

void writeSomething(const QString &name, QSharedPointer<Resource> resource, int priority)
{
    qDebug() << name << "Starting to write something with priority" << priority << "...";

    // Send cancellation request to all lower levels.
//    QVector<QSharedPointer<CancellationRequest>> cancellationRequests;
//    for (int prio = priority+1; prio <= QThread::InheritPriority; prio++)
//    {
//        QSharedPointer<CancellationRequest> r(new CancellationRequest(QString(resource->name() + QStringLiteral("-cancellation-") + QString::number(prio)).toStdString()));
//        cancellationRequests.append(r);

//        qInfo() << name << "[prio" << priority << "] Sending cancellation request to prio" << prio;
//        r->request();
//    }

    // Acquire lock
    qDebug() << name << "Acquiring the write lock...";
    ReadWriteLocker lock(resource->rwLock(), ReadWriteLock::LockMethod::WRITE);
    qDebug() << name << "Acquired the write lock.";

//    CancellationRequest cancellation(QString(resource->name() + QStringLiteral("-cancellation-") + QString::number(priority)).toStdString()); // Used to listen if some other app requested cancellation.

    // Write to the resource with intermediate checks if cancellation was requested.
    std::cout << name.toStdString() << " Writing to the resource";
    for (int i=0; i<10; i++)
    {
//        if (!cancellation.isRequested())
            resource->features().at(0)->doSomething();
//        else
//        {
//            qDebug() << name << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
//            break;
//        }
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedPointer<Resource> resource(new Resource("PreciousResource"));

    for(int i=0; i<5; i++)
    {
        QFuture<void> result = QtConcurrent::run(&readSomething, QString("reader %1").arg(i), resource, 1);
    }

    for(int i=0; i<5; i++)
    {
        QFuture<void> result = QtConcurrent::run(&writeSomething, QString("writer %1").arg(i), resource, 1);
    }

    return a.exec();
}
