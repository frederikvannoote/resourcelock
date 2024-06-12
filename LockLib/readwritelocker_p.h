#pragma once

#include "readwritelock.h"
#include <QString>


class ReadWriteLockerPrivate
{
public:
    ReadWriteLockerPrivate(ReadWriteLock &lock, const ReadWriteLock::LockMethod &method, const QString& context);
    virtual ~ReadWriteLockerPrivate();

    ReadWriteLock &m_lock;
    ReadWriteLock::LockMethod m_method;
    QString m_context;
};
