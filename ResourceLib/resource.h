#pragma once

#include "ResourceLib_global.h"
#include "resourcelock.h"
#include "readwritelock.h"
#include "feature.h"
#include <QSharedPointer>
#include <QList>


class RESOURCELIB_EXPORT Resource
{
public:
    Resource(const QString &name);

    QString name() const;

    ResourceLock lock(int priority = 0) const;
    ReadWriteLock &rwLock();

    QList<QSharedPointer<Feature>> features() const;

private:
    const QString m_name;
    ResourceLock m_lock;
    ReadWriteLock m_rwLock;
    const QList<QSharedPointer<Feature>> m_features;
};
