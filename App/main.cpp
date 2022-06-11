#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include "resource.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Resource resource("PreciousResource");

    // Section where we do something with the resource
    {
        ResourceLock lock = resource.lock(); // The ResourceLock unlocks when it goes out of scope.
        while (!lock.isValid())
        {
            qInfo() << "Could not get lock. Will wait for it...";
            QThread::sleep(1);
            lock = resource.lock();
        }

        // Do something with the resource
        resource.doSomething();
    }

    return 0;
}
