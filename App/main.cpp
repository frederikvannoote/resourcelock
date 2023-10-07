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


void readWriteLock(int prio, bool write, bool crash)
{
    const QString lock_path = "C:\\ProgramData\\devicelocks\\";

    // Set priority of the main thread to influence scheduling priority.
    const QThread::Priority priority = static_cast<QThread::Priority>(prio);
    QThread::currentThread()->setPriority(priority);
    qDebug() << "Set priority to" << priority;

    Resource resource(lock_path, QString("PreciousResource"));

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
        ReadWriteLocker lock(resource.rwLock(),
                             write ? ReadWriteLock::LockMethod::WRITE : ReadWriteLock::LockMethod::READ);

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

            if (crash)
                resource.features().at(1)->doSomething();
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

            if (crash)
                resource.features().at(1)->getSomething();
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
                                    QCoreApplication::translate("action", "Action"));
    actionOption.setDefaultValue(QStringLiteral("Read"));
    parser.addOption(actionOption);

    QCommandLineOption crashOption(QStringList() << "c" << "crash",
                                    QCoreApplication::translate("crash", "Crash"));
    parser.addOption(crashOption);

    parser.process(a);

    qDebug() << parser.value(actionOption).toLower();

    readWriteLock(parser.value(priorityOption).toInt(),
                  parser.value(actionOption).toLower() == QStringLiteral("write"),
                  parser.isSet(crashOption));

    return 0;
}
