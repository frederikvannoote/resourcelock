#include "resource.h"
#include <QDebug>
#include <QThread>


Resource::Resource(const QString &lockpath, const QString &name):
    m_name(name),
    m_rwLock(lockpath.toStdString() + name.toStdString() + "RW"),
    m_features({ QSharedPointer<Feature>(new Feature(m_rwLock)) })
{
}

QString Resource::name() const
{
    return m_name;
}

ReadWriteLock& Resource::rwLock()
{
    return m_rwLock;
}

QList<QSharedPointer<Feature> > Resource::features() const
{
    return m_features;
}
