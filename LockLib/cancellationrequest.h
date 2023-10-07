#pragma once

#include <memory>
#include <string>
class CancellationRequestPrivate;


class CancellationRequest
{
public:
    CancellationRequest(const std::string &name);
    CancellationRequest(const CancellationRequest &other);
    CancellationRequest(const CancellationRequest &&other);
    virtual ~CancellationRequest();

    CancellationRequest &operator=(CancellationRequest &other);
    CancellationRequest &operator=(CancellationRequest &&other);

    bool isRequested() const;

    void request();
    void undo();

private:
    std::shared_ptr<CancellationRequestPrivate> d_ptr;
};
