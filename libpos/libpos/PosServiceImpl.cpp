#include "pch.h"
#include <stdexcept>
#include "PosServiceImpl.h"
using namespace std;
using namespace std::chrono;

namespace libpos
{
    bool PosServiceImpl::isLadyNight() const
    {
        auto now = datetimeFunc();
        auto tt = system_clock::to_time_t(now);
        tm tm;
        localtime_s(&tm, &tt);
        return tm.tm_wday == 3;
    }

    PosServiceImpl::PosServiceImpl(const shared_ptr<SecurityService>& securityService, const function<system_clock::time_point()>& datetimeFunc) :
        securityService(securityService),
        datetimeFunc(datetimeFunc ? datetimeFunc : [] { return system_clock::now(); })
    {
    }

    size_t PosServiceImpl::calcPrice(const UserItem& user) const
    {
        if (securityService && securityService->issued(user))
            throw IssuedUserException("invalid user.");
        switch (user.gender)
        {
        case libpos::Genders::male:
            return isLadyNight() ? 600 : 300;
        case libpos::Genders::female:
            return isLadyNight() ? 0 : 600;
        default:
            throw runtime_error("invalid operation.");
        }
    }
}
