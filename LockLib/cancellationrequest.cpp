#include "cancellationrequest.h"
#include "cancellationrequest_p.h"
#include <QDebug>


CancellationRequest::CancellationRequest(const QString &name):
    d_ptr(new CancellationRequestPrivate(name))
{
}

CancellationRequest::CancellationRequest(const CancellationRequest &other):
    d_ptr(other.d_ptr)
{
}

CancellationRequest::~CancellationRequest()
{
}

CancellationRequest &CancellationRequest::operator=(CancellationRequest &&other)
{
    if (this != &other) {
        qSwap(d_ptr, other.d_ptr);
    }
    return *this;
}

bool CancellationRequest::isRequested() const
{
    Q_D(const CancellationRequest);
    return d->isRequested();
}

void CancellationRequest::request()
{
    Q_D(CancellationRequest);
    return d->request();
}

void CancellationRequest::undo()
{
    Q_D(CancellationRequest);
    return d->undo();
}

CancellationRequestPrivate::CancellationRequestPrivate(const QString &name):
    m_mutex(NULL),
    m_name(name),
    m_weOwnTheLock(false)
{
    const BOOL initialOwner = FALSE;    // thread will not own the mutex after calling CreateMutexA, so no ReleaseMutex call needed when mutex was not locked
    if (name.size() > 0)
    {
        m_mutex = CreateMutexA(nullptr, initialOwner, name.toStdString().c_str());
        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_mutex == NULL)
        {
            qWarning() << "------ failed to create mutex" << name.toStdString().c_str() << m_mutex;
        }
    }
}

CancellationRequestPrivate::~CancellationRequestPrivate()
{
    undo();
}

bool CancellationRequestPrivate::isRequested() const
{
    if (m_mutex)
    {
        DWORD event = WaitForSingleObjectEx(m_mutex, 0, FALSE);
        {
            if (event == WAIT_OBJECT_0)
            {
                ReleaseMutex(m_mutex);
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

void CancellationRequestPrivate::request()
{
    if (m_mutex)
    {
        DWORD event = WaitForSingleObjectEx(m_mutex, 0, FALSE);
        switch (event)
        {
        case WAIT_OBJECT_0:
            // We got hold of the mutex
            qInfo() << "We got hold of the mutex";
            m_weOwnTheLock = true;
            break;

        case WAIT_TIMEOUT:
            // Timeout while waiting. The mutex is in non-signaled state.
            qInfo() << "Some other application is already requesting cancellation.";
            break;

        case WAIT_ABANDONED:
            // We got hold of an abandoned mutex.
            qCritical() << "We got hold of an abandoned mutex:" << m_name;
            break;

        default:
            qInfo() << "CancellationRequestPrivate WaitForSingleObjectEx" << m_name << event;
        }
    }
}

void CancellationRequestPrivate::undo()
{
    if (m_mutex && m_weOwnTheLock)
    {
        BOOL success = ReleaseMutex(m_mutex);
        if (success == TRUE)
        {
            qInfo() << "Stopped requesting cancellation of prio" << m_name;
        }
        else
        {
            qWarning() << "------ failed to unlock mutex" << m_name << m_mutex << "last error" << GetLastError();
        }
    }
}
