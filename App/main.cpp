#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QVector>
#include <QSharedPointer>
#include <QCommandLineParser>
#include "resource.h"
#include "cancellationrequest.h"
#include "readwritelock.h"
#include "readwritelocker.h"


void readWriteLock(int prio, bool write)
{
    // Set priority of the main thread to influence scheduling priority.
    const QThread::Priority priority = static_cast<QThread::Priority>(prio);
    QThread::currentThread()->setPriority(priority);
    qDebug() << "Set priority to" << priority;

    Resource resource("PreciousResource");

    // Section where we do something with the resource
    {
        // Send cancellation request to all lower levels.
        QVector<QSharedPointer<CancellationRequest>> cancellationRequests;
        for (int prio = priority+1; prio <= QThread::InheritPriority; prio++)
        {
            QSharedPointer<CancellationRequest> r(new CancellationRequest(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(prio)).toStdString()));
            cancellationRequests.append(r);

            qInfo() << "[prio" << priority << "] Sending cancellation request to prio" << prio;
            r->request();
        }

        // Acquire lock
        qDebug() << "Acquiring the lock...";
        ReadWriteLocker lock(resource.rwLock(),
                             write ? ReadWriteLock::LockMethod::WRITE : ReadWriteLock::LockMethod::READ);
        qDebug() << "Acquired the lock.";

        CancellationRequest cancellation(QString(resource.name() + QStringLiteral("-cancellation-") + QString::number(priority)).toStdString());

        // Do something with the resource
        if (write)
        {
            for (int i=0; i<10; i++)
            {
                if (!cancellation.isRequested())
                    resource.features().at(0)->doSomething();
                else
                {
                    qDebug() << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
                    break;
                }
            }
        }
        else
        {
            for (int i=0; i<10; i++)
            {
                if (!cancellation.isRequested())
                    resource.features().at(0)->getSomething();
                else
                {
                    qDebug() << "[prio" << priority << "] Not executing this step. Cancellation was requested.";
                    break;
                }
            }
        }
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

    qDebug() << parser.value(actionOption).toLower();

    readWriteLock(parser.value(priorityOption).toInt(),
                  parser.value(actionOption).toLower() == QStringLiteral("write"));

    return 0;
}
