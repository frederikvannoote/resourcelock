#pragma once

#include "readwritelock.h"
#include "feature.h"
#include <QSharedPointer>
#include <QList>


class Resource
{
public:
    Resource(const QString &lockpath, const QString &name);

    QString name() const;

    ReadWriteLock &rwLock();

    QList<QSharedPointer<Feature>> features() const;

private:
    const QString m_name;
    ReadWriteLock m_rwLock;
    const QList<QSharedPointer<Feature>> m_features;
};
