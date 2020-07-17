#pragma once
#include <libpos/libpos.h>

namespace libpos
{
    class PosServiceImpl : public PosService
    {
    protected:

        std::shared_ptr<SecurityService> securityService;

        std::function<std::chrono::system_clock::time_point()> datetimeFunc;

        bool isLadyNight() const;

    public:

        PosServiceImpl(const std::shared_ptr<SecurityService>& securityService, const std::function<std::chrono::system_clock::time_point()>& datetimeFunc);

        virtual std::size_t calcPrice(const UserItem& user) const override;
    };
}
