#pragma once

#include <QObject>
#include <Windows.h>


class ResourceLockWatcher: public QObject
{
    Q_OBJECT

public:
    ResourceLockWatcher(const QString &name, QObject *parent = nullptr);

    bool isLocked() const;

private:
    HANDLE m_mutex;
    const QString m_name;
};
