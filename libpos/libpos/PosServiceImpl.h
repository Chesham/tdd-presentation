#pragma once
#include <libpos/libpos.h>

namespace libpos
{
    class PosServiceImpl : public PosService
    {
    protected:

        std::shared_ptr<SecurityService> securityService;

    public:

        PosServiceImpl(const std::shared_ptr<SecurityService>& securityService);

        virtual std::size_t calcPrice(const UserItem& user) const override;
    };
}
