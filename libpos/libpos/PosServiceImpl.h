#pragma once
#include <libpos/libpos.h>

namespace libpos
{
    class PosServiceImpl : public PosService
    {
    protected:

        PosServiceConfiguration config;

        bool isLadyNight() const;

    public:

        PosServiceImpl(const PosServiceConfiguration& config);

        virtual std::size_t calcPrice(const UserItem& user) const override;

        virtual std::shared_ptr<std::function<void(std::shared_future<std::size_t>&)>> calcPrice(const UserItem& user, const std::function<void(std::shared_future<std::size_t>&)>& completeEvent) const override;
    };
}
