#include "resource.h"
#include <QDebug>
#include <QThread>


Resource::Resource(const QString &name):
    m_name(name)
{
}

ResourceLock Resource::lock(int priority) const
{
    Q_UNUSED(priority);
    return ResourceLock(m_name);
}

bool Resource::doSomething()
{
    ResourceLock l = lock();
    return doSomething(l);
}

bool Resource::doSomething(ResourceLock &lock)
{
    if (!lock.isValid())
    {
        qWarning() << "Attempt to access resource" << m_name << "with invalid lock";
        return false;
    }

    qDebug() << "Do something with" << m_name;
    QThread::sleep(10);
    qDebug() << "Done something with" << m_name;

    return true;
}

int Resource::getSomething() const
{
    ResourceLock l = lock();
    return getSomething(l);
}

int Resource::getSomething(ResourceLock &lock) const
{
    if (!lock.isValid())
    {
        qWarning() << "Attempt to access resource" << m_name << "with invalid lock";
        return false;
    }

    return 42;
}
