#include "cancellationrequest.h"
#include "cancellationrequest_p.h"
#include <iostream>


CancellationRequest::CancellationRequest(const std::string &name):
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

CancellationRequest& CancellationRequest::operator=(CancellationRequest &&other)
{
    if (this != &other) {
        d_ptr = std::move(other.d_ptr);
    }
    return *this;
}

bool CancellationRequest::isRequested() const
{
    return d_ptr->isRequested();
}

void CancellationRequest::request()
{
    return d_ptr->request();
}

void CancellationRequest::undo()
{
    return d_ptr->undo();
}

CancellationRequestPrivate::CancellationRequestPrivate(const std::string &name):
    m_mutex(NULL),
    m_name(name),
    m_weOwnTheLock(false)
{
    const BOOL initialOwner = FALSE;    // thread will not own the mutex after calling CreateMutexA, so no ReleaseMutex call needed when mutex was not locked
    if (name.size() > 0)
    {
        m_mutex = CreateMutexA(nullptr, initialOwner, name.c_str());
        // If handle is NULL, the lock() and unlock() functions will be no-ops.
        if (m_mutex == NULL)
        {
            std::cerr << "Failed to create mutex " << name << " " << m_mutex << std::endl;
        }
    }
}

CancellationRequestPrivate::CancellationRequestPrivate(const CancellationRequestPrivate &other):
    m_mutex(other.m_mutex),
    m_name(other.m_name),
    m_weOwnTheLock(other.m_weOwnTheLock)
{
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
            std::cout << "We got hold of the mutex" << std::endl;
            m_weOwnTheLock = true;
            break;

        case WAIT_TIMEOUT:
            // Timeout while waiting. The mutex is in non-signaled state.
            std::cout << "Some other application is already requesting cancellation." << std::endl;
            break;

        case WAIT_ABANDONED:
            // We got hold of an abandoned mutex.
            std::cerr << "We got hold of an abandoned mutex: " << m_name << std::endl;
            break;

        default:
            std::cout << "CancellationRequestPrivate WaitForSingleObjectEx " << m_name << " " << event << std::endl;
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
            std::cout << "Stopped requesting cancellation of prio " << m_name << std::endl;
        }
        else
        {
            std::cerr << "Failed to unlock mutex " << m_name << " " << m_mutex << " last error " << GetLastError() << std::endl;
        }
    }
}
