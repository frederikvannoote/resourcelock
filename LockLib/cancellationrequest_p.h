#pragma once

#include <QString>
#include <QAtomicInteger>
#include <Windows.h>

class CancellationRequestPrivate
{
public:
    CancellationRequestPrivate(const QString &name);
    virtual ~CancellationRequestPrivate();

    bool isRequested() const;

    void request();
    void undo();

private:
    HANDLE m_mutex = NULL;
    const QString m_name;
    QAtomicInteger<bool> m_isLocked;
    bool m_weOwnTheLock;
};
