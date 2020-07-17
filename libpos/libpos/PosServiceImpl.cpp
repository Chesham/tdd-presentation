#include "pch.h"
#include <stdexcept>
#include "PosServiceImpl.h"
using namespace std;

namespace libpos
{
    PosServiceImpl::PosServiceImpl(const shared_ptr<SecurityService>& securityService) :
        securityService(securityService)
    {
    }

    size_t PosServiceImpl::calcPrice(const UserItem& user) const
    {
        if (securityService && securityService->issued(user))
            throw IssuedUserException("invalid user.");
        switch (user.gender)
        {
        case libpos::Genders::male:
            return 300;
        case libpos::Genders::female:
            return 600;
        default:
            throw runtime_error("invalid operation.");
        }
    }
}
