#pragma once

#include <string>
#include <Windows.h>

class CancellationRequestPrivate
{
public:
    CancellationRequestPrivate(const std::string &name);
    CancellationRequestPrivate(const CancellationRequestPrivate &name);
    virtual ~CancellationRequestPrivate();

    bool isRequested() const;

    void request();
    void undo();

private:
    HANDLE m_mutex = NULL;
    const std::string m_name;
    bool m_weOwnTheLock;
};
