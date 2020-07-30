#pragma once
#include "useritem.h"
#include <stdexcept>
#include <functional>
#include <memory>

namespace libpos
{
    class IssuedUserException : public std::runtime_error
    {
    public:

        IssuedUserException(const char* msg) : runtime_error(msg) { }
    };

    struct SecurityService
    {
        virtual ~SecurityService() { }

        virtual bool issued(const UserItem& user) const = 0;

        virtual void issued(const UserItem& user, std::weak_ptr<std::function<void(bool)>> completeEvent) const = 0;
    };
}
