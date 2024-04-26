#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QVector>
#include <QSharedPointer>
#include <QCommandLineParser>
#include <iostream>
#include "resource.h"
#include "cancellationrequest.h"
#include "readwritelock.h"
#include "readwritelocker.h"


void readSomething(Resource &resource, int priority)
{
    qDebug() << "Starting to read something with priority" << priority << "...";

//    // Send cancellation request to all lower levels.
//    QVector<QSharedPointer<CancellationRequest>> cancellationRequests;
//    for (int prio = priority+1; prio <= QThread::InheritPriority; prio++)
//    {
//        QSharedPointer<CancellationRequest> r(new CancellationRequest(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(prio)).toStdString()));
//        cancellationRequests.append(r);

//        qInfo() << "[prio" << priority << "] Sending cancellation request to prio" << prio;
//        r->request();
//    }

    // Acquire lock
    qDebug() << "Acquiring the read lock...";
    ReadWriteLocker locker(resource.rwLock(), "main thread", ReadWriteLock::LockMethod::READ);
    qDebug() << "Acquired the read lock.";

//    CancellationRequest cancellation(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(priority)).toStdString());

    // Read the resource with intermediate checks if cancellation was requested.
    for (int i=0; i<10; i++)
    {
//        if (!cancellation.isRequested())
        {
            qInfo() << "Reading from the resource, i" << i;
            resource.features().at(0)->getSomething(locker, "main thread");
        }
//        else
//        {
//            qDebug() << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
//            break;
//        }
    }
}

void writeSomething(Resource &resource, int priority)
{
    qDebug() << "Starting to write something with priority" << priority << "...";

//    // Send cancellation request to all lower levels.
//    QVector<QSharedPointer<CancellationRequest>> cancellationRequests;
//    for (int prio = priority+1; prio <= QThread::InheritPriority; prio++)
//    {
//        QSharedPointer<CancellationRequest> r(new CancellationRequest(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(prio)).toStdString()));
//        cancellationRequests.append(r);

//        qInfo() << "[prio" << priority << "] Sending cancellation request to prio" << prio;
//        r->request();
//    }

    // Acquire lock
    qDebug() << "Acquiring the write lock...";
    ReadWriteLocker locker(resource.rwLock(), "main thread", ReadWriteLock::LockMethod::WRITE);
    qDebug() << "Acquired the write lock.";

//    CancellationRequest cancellation(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(priority)).toStdString()); // Used to listen if some other app requested cancellation.

    // Write to the resource with intermediate checks if cancellation was requested.
    for (int i=0; i<10; i++)
    {
//        if (!cancellation.isRequested())
        {
            qInfo() << "Writing to the resource, i" << i;
            resource.features().at(0)->doSomething(locker, "main thread");
        }
//        else
//        {
//            qDebug() << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
//            break;
//        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion(QStringLiteral("1.0"));

    QCommandLineParser parser;
    parser.setApplicationDescription("Priority system lock demo");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption priorityOption(QStringList() << "p" << "priority",
                                      QCoreApplication::translate("priority", "Lock priority"),
                                      QCoreApplication::translate("priority", "priority"));
    priorityOption.setDefaultValue(QString::number(QThread::Priority::NormalPriority));
    parser.addOption(priorityOption);

    QCommandLineOption actionOption(QStringList() << "a" << "action",
                                    QCoreApplication::translate("action", "Action"),
                                    QCoreApplication::translate("action", "action"));
    actionOption.setDefaultValue(QStringLiteral("Read"));
    parser.addOption(actionOption);

    parser.process(a);

    // Set priority of the main thread to influence scheduling priority.
    const int priority = parser.value(priorityOption).toInt();
    const QThread::Priority threadPriority = static_cast<QThread::Priority>(priority);
    QThread::currentThread()->setPriority(threadPriority);
    qDebug() << "Set priority to" << threadPriority;

    Resource resource("PreciousResource");
    const bool write = parser.value(actionOption).toLower() == QStringLiteral("write");

    // Section where we do something with the resource
    if (write)
        writeSomething(resource, priority);
    else
        readSomething(resource, priority);

    return 0;
}
