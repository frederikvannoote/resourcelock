#pragma once

#include "LockLib_global.h"
#include <QSharedPointer>
class CancellationRequestPrivate;


class LOCKLIB_EXPORT CancellationRequest
{
public:
    CancellationRequest(const QString &name);
    CancellationRequest(const CancellationRequest &other);
    CancellationRequest(const CancellationRequest &&other);
    virtual ~CancellationRequest();

    CancellationRequest &operator=(CancellationRequest &other);
    CancellationRequest &operator=(CancellationRequest &&other);

    bool isRequested() const;

    void request();
    void undo();

private:
    Q_DECLARE_PRIVATE(CancellationRequest)
    QSharedPointer<CancellationRequestPrivate> d_ptr;
};
