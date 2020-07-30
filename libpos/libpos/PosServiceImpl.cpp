#include "pch.h"
#include <stdexcept>
#include "PosServiceImpl.h"
using namespace std;
using namespace std::chrono;

namespace libpos
{
    bool PosServiceImpl::isLadyNight() const
    {
        auto now = config.datetimeFunc();
        auto tt = system_clock::to_time_t(now);
        tm tm;
        localtime_s(&tm, &tt);
        return tm.tm_wday == 3;
    }

    PosServiceImpl::PosServiceImpl(const PosServiceConfiguration& config) :
        config(config)
    {
    }

    size_t PosServiceImpl::calcPrice(const UserItem& user) const
    {
        auto& securityService = config.securityService;
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

    shared_ptr<function<void(shared_future<size_t>&)>> PosServiceImpl::calcPrice(const UserItem& user, const function<void(shared_future<size_t>&)>& completeEvent) const
    {
        auto prom = make_shared<promise<size_t>>();
        auto calc = [&](const UserItem& user)
        {
            switch (user.gender)
            {
            case libpos::Genders::male:
                return isLadyNight() ? 600 : 300;
            case libpos::Genders::female:
                return isLadyNight() ? 0 : 600;
            default:
                throw runtime_error("invalid operation.");
            }
        };
        auto calcAndSet = [&](const UserItem& user, promise<size_t>& prom)
        {
            try
            {
                prom.set_value(calc(user));
            }
            catch (const std::exception&)
            {
                prom.set_exception(current_exception());
            }
        };
        auto calcEvent = make_shared<function<void(shared_future<size_t>&)>>([&](shared_future<size_t>& price)
            {
                completeEvent(price);
            });
        weak_ptr<decltype(calcEvent)::element_type> weakCalcEvent(calcEvent);
        shared_ptr<function<void(bool)>> securityEvent;
        auto& securityService = config.securityService;
        if (securityService)
        {
            securityEvent = make_shared<function<void(bool)>>([=](bool issued) mutable
                {
                    auto e = weakCalcEvent.lock();
                    if (!e)
                        return;
                    auto fut = (shared_future<size_t>)prom->get_future();
                    if (issued)
                    {
                        prom->set_exception(make_exception_ptr(IssuedUserException("invalid user.")));
                        (*e)(fut);
                        return;
                    }
                    calcAndSet(user, *prom);
                    (*e)(fut);
                });
            securityService->issued(user, securityEvent);
            return calcEvent;
        }
        calcAndSet(user, *prom);
        auto fut = (shared_future<size_t>)prom->get_future();
        completeEvent(fut);
        return nullptr;
    }
}
