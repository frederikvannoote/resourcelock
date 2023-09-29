#pragma once

#include "ResourceLib_global.h"
#include "readwritelock.h"
#include "feature.h"
#include <QSharedPointer>
#include <QList>


class RESOURCELIB_EXPORT Resource
{
public:
    Resource(const QString &name);

    QString name() const;

    ReadWriteLock &rwLock();

    QList<QSharedPointer<Feature>> features() const;

private:
    const QString m_name;
    ReadWriteLock m_rwLock;
    const QList<QSharedPointer<Feature>> m_features;
};
