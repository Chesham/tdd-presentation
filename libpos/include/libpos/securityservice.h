#pragma once
#include "useritem.h"
#include <stdexcept>

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
    };
}
